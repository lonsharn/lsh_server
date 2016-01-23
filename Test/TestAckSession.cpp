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
	if(!mq->InitByJsonString(string("TestClientSession.json")))
	{
		return -1;
	}

	HMQConnection* conn = mq->getConn("group1");
	if(!conn)
	{
		return -1;
	}

	while(true)
	{
		HMQSession* session = conn->CreateSession("TestServerSession", SessionType_Ack);
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
					log4cpp::Category::getInstance("LoginService").infoStream()<<"TestClientSession Receive Message!"<<
					"Timestamp="<<msg->GetTimestamp()<<"SenderID="<<msg->GetSenderID()<<"ReceiverID="<<msg->GetReceiverID();
				});
	}
	sleep(-1);
	return 0;
}
