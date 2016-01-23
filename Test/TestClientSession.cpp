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

#include "Utility/log4cpp_wrapper.h"
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
	if(!mq->InitByJsonString(string("TestClientSession.json")))
	{
		return -1;
	}

	HMQConnection* conn = mq->getConn("group1");
	if(!conn)
	{
		return -1;
	}

	{
		HMQSession* session = conn->CreateSession("TestServerSession", SessionType_Client);
		if(!session)
		{
			return -1;
		}
		auto msg = session->CreateHMQTextMessage();
		if(!msg)
		{
			return -1;
		}

		msg->SetText("Hello! This is TestClientSession Speaking!");

		session->SendMessage(msg);
		session->SetMessageListener([session](const HMQMessage* msg)
				{
					INFO(LoginService)<<"TestClientSession Receive Message!"<<
					"Timestamp="<<msg->GetTimestamp()<<"SenderID="<<msg->GetSenderID()<<"ReceiverID="<<msg->GetReceiverID();
				});
	}

	/*
	{
		HMQSession* session = conn->CreateSession("TestExclusiveSession", SessionType_Client);
		if(!session)
		{
			return -1;
		}
		auto msg = session->CreateHMQTextMessage();
		if(!msg)
		{
			return -1;
		}

		msg->SetText("Hello! This is TestClientSession Speaking!");

		session->SendMessage(msg);
		session->SetMessageListener([](const HMQMessage* msg)
				{
				cout<<"TestClientSession Receive Message!"<<endl;
				cout<<"Timestamp="<<msg->GetTimestamp()<<endl;
				cout<<"SenderID="<<msg->GetSenderID()<<endl;
				cout<<"ReceiverID="<<msg->GetReceiverID()<<endl;
				cout<<"File="<<__FILE__", Line="<<__LINE__<<endl;
				});
	}

	{
		HMQSession* session = conn->CreateSession("TestTimeoutSession", SessionType_Client);
		if(!session)
		{
			return -1;
		}
		auto msg = session->CreateHMQTextMessage();
		if(!msg)
		{
			return -1;
		}

		msg->SetText("Hello! This is TestClientSession Speaking!");

		session->SendMessage(msg);
		session->SetMessageListener([](const HMQMessage* msg)
				{
				cout<<"TestClientSession Receive Message!"<<endl;
				cout<<"Timestamp="<<msg->GetTimestamp()<<endl;
				cout<<"SenderID="<<msg->GetSenderID()<<endl;
				cout<<"ReceiverID="<<msg->GetReceiverID()<<endl;
				cout<<"File="<<__FILE__", Line="<<__LINE__<<endl;
				});
	}
	*/

	sleep(-1);
	return 0;
}
