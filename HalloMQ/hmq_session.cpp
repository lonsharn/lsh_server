/**
 * @file hmq_session.cpp
 * @brief class HQMSession
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#include "hmq_session.h"
#include "hmq_connection.h"
#include "hmq_message.h"
#include "hmq_message_listener.h"
#include <uuid/uuid.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/MessageProducer.h>
#include <cms/MessageConsumer.h>
#include <log4cpp/Category.hh>
using namespace std;
/**************************Begin Of HMQSession******************************/
HMQSession::~HMQSession()
{
	Close();
	if(m_message_listener)
	{
		delete m_message_listener;
		m_message_listener = nullptr;
	}
}

HMQSession::HMQSession()
{
	uuid_t uid;
	uuid_generate(uid);
	char out[20] = {0};
	uuid_unparse(uid, out);
	m_uuid = out;
}

bool HMQSession::Init(HMQConnection* connection, const std::string& name, const std::string& pwd)
{
	m_connection = connection;
	m_session = m_connection->GetPooledSession();

	std::string strQueueName = m_connection->GetServiceName() + "" + name + "" + pwd;

	try
	{
		m_producer_dest = m_session->createQueue(strQueueName);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"create Producer Queue Error:"<<ex.getMessage();
		return false;
	}

	try
	{
		m_producer = m_session->createProducer(m_producer_dest);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"createProducer Error:"<<ex.getMessage();
		return false;
	}
	strQueueName = name + "" + m_connection->GetServiceName() + "" + pwd;

	try
	{
		m_consumer_dest = m_session->createQueue(strQueueName);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"create Consumer Queue Error:"<<ex.getMessage();
		return false;
	}

	if(m_message_listener == nullptr)
	{
		m_message_listener = new HMQMessageListener();
	}

	if(!InitConsumer())
	{
		return false;
	}

	m_service_name = name;
	return true;
}
HMQTextMessage* HMQSession::CreateHMQTextMessage()
{
	if(m_session == nullptr)
	{
		return nullptr;
	}
	auto* msg = new HMQTextMessage(this);
	msg->SetSenderID(m_uuid);
	return msg;
}
HMQMapMessage* HMQSession::CreateHMQMapMessage()
{
	if(m_session == nullptr)
	{
		return nullptr;
	}
	auto* msg = new HMQMapMessage(this);
	msg->SetSenderID(m_uuid);
	return msg;
}
void HMQSession::SetMessageListener(std::function<void(const HMQMessage*)>&& handler)
{
	if(!m_connection->IsStart())
	{
		return;
	}
	m_message_listener->SetMessageListener(std::move(handler));
	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").infoStream()<<"setMessageListener Error:"<<ex.getMessage();
	}
}
bool HMQSession::SendMessage(HMQMessage* msg)
{
	if(!m_connection->IsStart())
	{
		return false;
	}

	msg->SetReceiverID("0");

	try
	{
		m_producer->send(msg->GetMessage());
		log4cpp::Category::getInstance("HalloMQ").infoStream()<<"Send Message, Sender="<<m_uuid<<", Receiver=0, Timestamp="<<msg->GetTimestamp();
		return true;
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").infoStream()<<"Send Message Error, Error Message:"<<ex.getMessage();
		return false;
	}
}
bool HMQSession::SendMessageTo(const std::string& dest, HMQMessage* msg)
{
	if(!m_connection->IsStart())
	{
		return false;
	}

	msg->SetReceiverID(dest);
	try
	{
		m_producer->send(msg->GetMessage());
		log4cpp::Category::getInstance("HalloMQ").infoStream()<<"Send Message, Sender="<<m_uuid<<", Receiver=0, Timestamp="<<msg->GetTimestamp();
		return true;
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").infoStream()<<"Send Message to " <<dest<<" Error, Error Message:"<<ex.getMessage();
		return false;
	}
}
void HMQSession::Close()
{
	if(m_producer_dest != nullptr)
	{
		try
		{
			//delete m_producer_dest;
		}
		catch(const cms::CMSException& ex)
		{
			 log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Producer Dest error:"<<ex.getMessage();
		}

		m_producer_dest = nullptr;
	}

	if(m_consumer_dest != nullptr)
	{
		try
		{
			//delete m_consumer_dest;
		}
		catch(const cms::CMSException& ex)
		{
			 log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Consumer Dest error:"<<ex.getMessage();
		}

		m_consumer_dest = nullptr;
	}

	if(m_producer != nullptr)
	{
		try
		{
			delete m_producer;
		}
		catch(const cms::CMSException& ex)
		{
			log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Producer error:"<<ex.getMessage();
		}

		m_producer = nullptr;
	}

	if(m_consumer != nullptr)
	{
		try
		{
			m_consumer->setMessageListener(nullptr);
			delete m_consumer;
		}
		catch(const cms::CMSException& ex)
		{
			log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Consumer error:"<<ex.getMessage();
		}
		m_consumer = nullptr;
	}
}

HMQMessage* HMQSession::Wrapper(cms::Message* msg)
{
	if(!msg)
	{
		return nullptr;
	}

	HMQMessage* hmsg = nullptr;
	if(dynamic_cast<cms::TextMessage*>(msg))
	{
		hmsg = new HMQTextMessage();
	}
	else if(dynamic_cast<cms::MapMessage*>(msg))
	{
		hmsg = new HMQMapMessage();
	}
	else
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Unsurport Message Type!";
	}

	hmsg->SetMessage(const_cast<cms::Message*>(msg));
}

HMQMessage* HMQSession::Receive()
{
	if(!m_connection->IsStart())
	{
		return nullptr;
	}

	cms::Message* msg = nullptr;
	try
	{
		msg = m_consumer->receive();
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Receive Message error:"<<ex.getMessage();
		return nullptr;
	}

	return Wrapper(msg);
}

HMQMessage* HMQSession::Receive(long timeout)
{
	if(!m_connection->IsStart())
	{
		return nullptr;
	}

	cms::Message* msg = nullptr;
	try
	{
		msg = m_consumer->receive(timeout);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Receive Message error:"<<ex.getMessage();
		return nullptr;
	}

	return Wrapper(msg);
}

HMQMessage* HMQSession::ReceiveNoWait()
{
	if(!m_connection->IsStart())
	{
		return nullptr;
	}

	cms::Message* msg = nullptr;
	try
	{
		msg = m_consumer->receiveNoWait();
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Receive Message error:"<<ex.getMessage();
		return nullptr;
	}

	return Wrapper(msg);
}
/**************************Begin Of ClientSession******************************/
bool ClientSession::InitConsumer()
{
	std::string selector = "JMSCorrelationID='" + m_uuid + "'";
	try
	{
		m_consumer = m_session->createConsumer(m_consumer_dest, selector);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Create Consumer error!"<<ex.getMessage();
		return false;
	}

	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setMessageListener error!"<<ex.getMessage();
		return false;
	}

	log4cpp::Category::getInstance("HalloMQ").info("QueueName=%s, Selector=%s", m_consumer_dest->getQueueName().c_str(), selector.c_str());
	return true;
}
/**************************Begin Of ServerSession******************************/
bool ServerSession::InitConsumer()
{
	std::string selector = "JMSCorrelationID='" + m_uuid + "' OR JMSCorrelationID='0'";
	try
	{
		m_consumer = m_session->createConsumer(m_consumer_dest, selector);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Create Consumer error!"<<ex.getMessage();
		return false;
	}

	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setMessageListener error!"<<ex.getMessage();
		return false;
	}

	log4cpp::Category::getInstance("HalloMQ").info("QueueName=%s, Selector=%s", m_consumer_dest->getQueueName().c_str(), selector.c_str());
	return true;
}
/*************************Begin Of ExclusiveSession****************************/
bool ExclusiveSession::InitConsumer()
{
	try
	{
		m_consumer = m_session->createConsumer(m_consumer_dest);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Create Consumer error!"<<ex.getMessage();
		return false;
	}

	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setMessageListener error!"<<ex.getMessage();
		return false;
	}
	log4cpp::Category::getInstance("HalloMQ").info("QueueName=%s, Selector=All", m_consumer_dest->getQueueName().c_str());
	return true;
}
/**************************Begin Of TimeoutSession******************************/
void TimeoutSession::SetTimeout(long to)
{
	m_timeout = to;
}

bool TimeoutSession::InitConsumer()
{
    time_t t = time(NULL);
    long value = t*1000 - m_timeout;
	std::string selector = "JMSTimestamp<" + std::to_string(value);
	try
	{
		m_consumer = m_session->createConsumer(m_consumer_dest, selector);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Create Consumer error!"<<ex.getMessage();
		return false;
	}

	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setMessageListener error!"<<ex.getMessage();
		return false;
	}
	log4cpp::Category::getInstance("HalloMQ").info("QueueName=%s, Selector=%s", m_consumer_dest->getQueueName().c_str(), selector.c_str());
	return true;
}

/**************************Begin Of AckSession******************************/
bool AckSession::InitConsumer()
{
	std::string selector = "JMSCorrelationID='" + m_uuid + "'";
	try
	{
		m_consumer = m_session->createConsumer(m_consumer_dest, selector);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Create Consumer error!"<<ex.getMessage();
		return false;
	}

	try
	{
		m_consumer->setMessageListener(m_message_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setMessageListener error!"<<ex.getMessage();
		return false;
	}

	log4cpp::Category::getInstance("HalloMQ").info("QueueName=%s, Selector=%s", m_consumer_dest->getQueueName().c_str(), selector.c_str());
	return true;
}
