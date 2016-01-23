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
	class Message;
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
		HMQSession();
		virtual bool Init(HMQConnection* connection, const std::string& name, const std::string& pwd);

		virtual ~HMQSession();

		const std::string& GetSSID() const;
		HMQTextMessage* CreateHMQTextMessage();
		HMQMapMessage* CreateHMQMapMessage();
		bool SendMessage(HMQMessage* msg);
		bool SendMessageTo(const std::string& dest, HMQMessage* msg);
		void SetMessageListener(std::function<void(const HMQMessage*)>&& handler);
		virtual void SetTimeout(long to){};

		cms::Session* GetSession();

		virtual bool InitConsumer(){return true;};
		virtual void Close();
		std::string& GetServiceName();
		bool IsStart();

		HMQMessage* Wrapper(cms::Message* msg);
		HMQMessage* Receive();
		HMQMessage* Receive(long);
		HMQMessage* ReceiveNoWait();
	protected:
		HMQConnection* m_connection = nullptr;
		cms::Session* m_session = nullptr;
		std::string   m_uuid;

		cms::Queue*	m_producer_dest;
		cms::Queue*	m_consumer_dest;

		std::string m_service_name;

		cms::MessageProducer* m_producer = nullptr;
		cms::MessageConsumer* m_consumer = nullptr;

		HMQMessageListener*		m_message_listener = nullptr;
		std::atomic<bool>		m_started;
};

inline bool HMQSession::IsStart()
{
	return m_started;
}

inline std::string& HMQSession::GetServiceName()
{
	return m_service_name;
}

inline cms::Session* HMQSession::GetSession()
{
	return m_session;
}

inline const std::string& HMQSession::GetSSID() const
{
	return m_uuid;
}

/**************************Begin Of ClientSession******************************/
class ClientSession: public HMQSession
{
	public:
		ClientSession() = default;
		~ClientSession() = default;
	private:
		bool InitConsumer();
};

/**************************Begin Of ServerSession******************************/
class ServerSession: public HMQSession
{
	public:
		ServerSession()=default;
		~ServerSession()=default;
	private:
		bool InitConsumer();
};

/**************************Begin Of ExclusiveSession******************************/
class ExclusiveSession: public HMQSession
{
	public:
		ExclusiveSession() = default;
		~ExclusiveSession() = default;
	private:
		bool InitConsumer();
};

/**************************Begin Of TimeoutSession******************************/
class TimeoutSession: public HMQSession
{
	public:
		TimeoutSession() = default;
		~TimeoutSession() = default;
		void SetTimeout(long to);
		bool InitConsumer();
	private:
		long m_timeout;
};

/**************************Begin Of AckSession******************************/
class AckSession: public HMQSession
{
	public:
		AckSession() = default;
		~AckSession() = default;
		bool InitConsumer();
};
