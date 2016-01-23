/*************************************************************************
	> File Name: LoginService/main.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月14日 星期五 18时38分04秒
 ************************************************************************/


#include<unistd.h>
#include<string.h>
#include<iostream>
#include<map>

#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Category.hh>

#include "HalloSql/_kcsql.h"
#include "HalloMQ/hallo_mq.h"
#include "HalloMQ/hmq_connection.h"
#include "HalloMQ/hmq_session.h"
#include "HalloMQ/hmq_message.h"

using namespace std;

int main(int argc, char** argv)
{
	try
	{
		log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
	}
	catch (log4cpp::ConfigureFailure& f)
	{
		std::cout << "Configure Problem " << f.what() << std::endl;
		return -1;
	}

	HalloMQ* mq = HalloMQ::getInstance();
	if(!mq->InitByJsonString(string("TestServerSession.json")))
	{
		return -1;
	}

	HMQConnection* conn = mq->getConn("group1");
	if(!conn)
	{
		return -1;
	}

	HMQSession* session = conn->CreateSession("TestClientSession", SessionType_Server);
	if(!session)
	{
		return -1;
	}

	session->SetMessageListener([conn](const HMQMessage* _msg)
	{
		HMQTextMessage* tmsg = dynamic_cast<HMQTextMessage*>(const_cast<HMQMessage*>(_msg));
		cout<<"TestServerSession Receive Message:"<<tmsg->GetText()<<", SenderID="<<_msg->GetSenderID()<<", ReceiverID="<<_msg->GetReceiverID()<<endl;

		HMQSession* session = conn->CreateSession("TestClientSession", SessionType_Client);
		if(!session)
		{
			log4cpp::Category::getInstance("LoginService").errorStream()<<"Create Session To"<<_msg->GetSenderID()<<" Error!";
			return;
		}

		auto msg = session->CreateHMQTextMessage();
		if(!msg)
		{
			log4cpp::Category::getInstance("LoginService").errorStream()<<"Create Message To"<<_msg->GetSenderID()<<" Error!";
			return;
		}
		session->SetMessageListener([session](const HMQMessage* _msg)
				{
					HMQTextMessage* tmsg = dynamic_cast<HMQTextMessage*>(const_cast<HMQMessage*>(_msg));
					cout<<"TestServerSession Receive Message:"<<tmsg->GetText()<<", SenderID="<<_msg->GetSenderID()<<", ReceiverID="<<_msg->GetReceiverID()<<endl;
					auto msg = session->CreateHMQTextMessage();
					if(!msg)
					{
						log4cpp::Category::getInstance("LoginService").errorStream()<<"Create Message To"<<_msg->GetSenderID()<<" Error!";
						return;
					}

					if(!session->SendMessageTo(_msg->GetSenderID(), msg))
					{
						log4cpp::Category::getInstance("LoginService").errorStream()<<"Send Message To"<<_msg->GetSenderID()<<" Error!";
					}

					delete msg;
					return;
				});

		if(!session->SendMessageTo(_msg->GetSenderID(), msg))
		{
			log4cpp::Category::getInstance("LoginService").errorStream()<<"Send Message To"<<_msg->GetSenderID()<<" Error!";
		}

		delete msg;
		return;
	});

	sleep(-1);
	return 0;
}
