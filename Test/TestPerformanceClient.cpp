/*************************************************************************
	> File Name: LoginService/main.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月14日 星期五 18时38分04秒
 ************************************************************************/
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/syscall.h>
#include <pthread.h>
#include<string.h>
#include<iostream>
#include<map>
#include<memory>
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
	long count = 0;
	while(true)
	{
		HMQSession* session = conn->CreateSession("TestServerSession", SessionType_Client);
		if(!session)
		{
			usleep(1000000);
			continue;
		}
		++count;
		auto msg = session->CreateHMQTextMessage();
		if(!msg)
		{
			//session->Close();
			usleep(1000000);
			continue;
		}
		
		msg->SetText("Hello! This is TestClientSession Speaking!");
		session->SetMessageListener([conn, session](const HMQMessage* _msg)
				{
					auto msg = session->CreateHMQTextMessage();
					if(!msg)
					{
						//session->Close();
						return;
					}
					cout<<"TestClientSession Receive Message! Timestamp="<<_msg->GetTimestamp()<<", SenderID="<<_msg->GetSenderID()<<", ReceiverID="<<_msg->GetReceiverID()<<endl;
					if(!session->SendMessageTo(_msg->GetSenderID(), msg))
					{
						//session->Close();
					}
					delete msg;
					//conn->RemoveSession(session);
				});
		if(!session->SendMessage(msg))
		{
		}
		delete msg;
		cout<<"-------------Client Session Number="<<count<<endl;
		if(count == 1500)
		{
			break;
		}
	}
	sleep(-1);
	return 0;
}
