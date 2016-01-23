/**
 * @file hmq_session.h
 * @brief class HQMSession
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#pragma once
#include <string>
#include <functional>
#include <string>
#include <atomic>

namespace cms
{
	class Queue;
	class Session;
	class MessageProducer;
	class MessageConsumer;
}
class HMQMessage;
class HMQTextMessage;
class HMQMapMessage;
class HMQConnection;
class HMQMessageListener;

enum SessionType
{
	SessionType_Invalid = 0,
	SessionType_Client,
	SessionType_Server,
	SessionType_Exclusive,
	SessionType_Timeout,
	SessionType_Ack,
	SessionType_Max
};

enum CreateSessionType
{
	CreateSessionType_New = 1,
	CreateSessionType_Default,
	CreateSessionType_Pool
};

/**************************Begin Of HMQSession******************************/
class HMQSession
{
	public:
		HMQTextMessage* CreateHMQTextMessage();
		HMQMapMessage* CreateHMQMapMessage();
		bool SendMessage(HMQMessage* msg);
		bool SendMessageTo(const std::string& dest, HMQMessage* msg);
		void SetMessageListener(std::function<void(const HMQMessage*)>&& handler);

		HMQMessage* Receive();
		HMQMessage* Receive(long);
		HMQMessage* ReceiveNoWait();
};
