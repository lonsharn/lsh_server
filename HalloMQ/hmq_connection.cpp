/**
 * @file hmq_connection.cpp
 * @brief class HMQConnection
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */

#include"hmq_session.h"
#include"hmq_connection.h"
#include"hmq_exception_listener.h"

#include<string>
#include <unistd.h>
#include <memory>

#include<json/json.h>
#include<log4cpp/Category.hh>
#include<cms/ConnectionFactory.h>
#include<cms/Connection.h>

#define Error(type) log4cpp::Category::getInstance(type).errorStream()<<__FILE__":"<<__LINE__
#define MAX_SESSION_SIZE 150

void HMQConnection::Close()
{
	m_started = false;
	for(auto it:m_session_list)
	{
		it.second->Close();
	}

	for(auto it :m_session_pool)
	{
		try
		{
			delete it;
		}
		catch(const cms::CMSException& ex)
		{
			log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Default Session error:"<<ex.getMessage();
		}
	}

	m_session_pool.clear();

	if(m_connection != nullptr)
	{
		try
		{
			m_connection->close();
		}
		catch(const cms::CMSException& ex)
		{
			log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Close Connection error:"<<ex.getMessage();
		}

		/*
		try
		{
			delete m_connection;
		}
		catch(const cms::CMSException& ex)
		{
			log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Delete Connection error:"<<ex.getMessage();
		}
		*/

		m_connection = nullptr;
	}
}

HMQConnection::~HMQConnection()
{
	Close();
	for(auto it:m_session_list)
	{
		delete it.second;
	}
	m_session_list.clear();
}

bool HMQConnection::Connect()
{
	std::unique_ptr<cms::ConnectionFactory> connection_factory;
	try
	{
		connection_factory = std::unique_ptr<cms::ConnectionFactory>(cms::ConnectionFactory::createCMSConnectionFactory(m_broker_url));
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"createCMSConnectionFactory error:"<<ex.getMessage();
		return false;
	}

	try
	{
		m_connection = connection_factory->createConnection();
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"createConnection error:"<<ex.getMessage();
		return false;
	}

	if(m_exception_listener == nullptr)
	{
		m_exception_listener = new HMQExceptionListener();
		m_exception_listener->SetExceptionListener(std::bind(&HMQConnection::OnException, this, std::placeholders::_1));
	}

	try
	{
		m_connection->setExceptionListener(m_exception_listener);
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"setExceptionListener error:"<<ex.getMessage();
		return false;
	}

	try
	{
		m_session_pool.clear();
		for(int i=0;i<MAX_SESSION_SIZE; ++i)
		{
			m_session_pool.push_back(m_connection->createSession());
		}
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"createDefaultSession error:"<<ex.getMessage();
		return false;
	}

	try
	{
		m_connection->start();
	}
	catch(const cms::CMSException& ex)
	{
		log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Connection::start() error:"<<ex.getMessage();
		return false;
	}

	m_started = true;
	RecoverSession();

	log4cpp::Category::getInstance("HalloMQ").info("service %s startup!", m_service_name.c_str());

	return true;
}

bool HMQConnection::Init(const Json::Value& cfgValue)
{
	log4cpp::Category::getInstance("HalloMQ").info("service %s startup ...............!", m_service_name.c_str());
	m_group_name = cfgValue["name"].asString();
	m_service_name = cfgValue["local"].asString();
	m_broker_url = cfgValue["url"].asString();
	m_retry_time = 20;
	m_retry_thread = nullptr;

	Json::Value lst = cfgValue["dest"];

	m_dest_service.clear();
	for(auto i = 0; i < lst.size(); ++i)
	{
		m_dest_service.insert(make_pair(lst[i]["name"].asString(), lst[i]["pwd"].asString()));
	}

	return Connect();
}

HMQSession* HMQConnection::CreateSession(const std::string&& service, short type, long timeout)
{
	if(!m_started)
	{
		return nullptr;
	}

	++m_id;

	std::string pwd;
	log4cpp::Category& log = log4cpp::Category::getInstance("HalloMQ");
	auto it = m_dest_service.find(service);
	if(it == m_dest_service.end())
	{
		log.error("create client session error:service name = %s", service.c_str());
		return nullptr;
	}

	pwd = it->second;
	if(type <= SessionType_Invalid || type >= SessionType_Max)
	{
		return nullptr;
	}

	HMQSession* session = nullptr;
	switch(type)
	{
		case SessionType_Client:
			{
				session = new ClientSession;
			}
			break;
		case SessionType_Server:
			{
				session = new ServerSession;
			}
			break;
		case SessionType_Exclusive:
			{
				session = new ExclusiveSession;
			}
			break;
		case SessionType_Timeout:
			{
				session = new TimeoutSession;
			}
			break;
		case SessionType_Ack:
			{
				session = new AckSession;
			}
			break;
		default:
			break;
	}

	session->SetTimeout(timeout);
	if(!session->Init(this, service, pwd))
	{
		delete session;
		return nullptr;
	}

	m_session_list.insert(std::make_pair(session->GetSSID(), session));
	return session;
}

void HMQConnection::RetryConnect()
{
	int retry = 0;
	//Close();
	log4cpp::Category::getInstance("HalloMQ").error("Reconnect to %s ...................", m_broker_url.c_str());
	while(true)
	{
		if(Connect())
		{
			log4cpp::Category::getInstance("HalloMQ").info("Reconnect to %s success!", m_broker_url.c_str());
			break;
		}

		++retry;
		log4cpp::Category::getInstance("HalloMQ").error("Connect to %s, retry %d times", m_broker_url.c_str(), retry);

		sleep(1);
	}
}

void HMQConnection::OnException(const cms::CMSException& ex)
{
	m_connection->setExceptionListener(nullptr);
	Close();
	log4cpp::Category::getInstance("HalloMQ").errorStream()<<"Connection::onException:"<<ex.getMessage();
	sleep(5);
	if(m_retry_thread)
	{
		delete m_retry_thread;
	}
	m_retry_thread = new std::thread(std::bind(&HMQConnection::RetryConnect, this));
	m_retry_thread->detach();
}

void HMQConnection::RecoverSession()
{
	for(auto it: m_session_list)
	{
		std::string& name = it.second->GetServiceName();
		std::string pwd	= m_dest_service[name];
		if(!it.second->Init(this, name, pwd))
		{
			log4cpp::Category::getInstance("HalloMQ").error(__FILE__":%d, Recover Session %s Failed!", __LINE__, name.c_str());
			continue;
		}
	}

	log4cpp::Category::getInstance("HalloMQ").info(__FILE__":%d, Recover %d Sessions!", __LINE__, m_session_list.size());
}

void HMQConnection::RemoveSession(HMQSession* session)
{
	if(session)
	{
		m_session_list.erase(session->GetSSID());
		session->Close();
	}
}

cms::Session* HMQConnection::GetPooledSession()
{
	if(!m_started)
	{
		return nullptr;
	}

	return m_session_pool[m_id%MAX_SESSION_SIZE];
}
