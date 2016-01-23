/**
 * @file hmq_message.h
 * @brief class HMQMessage
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#pragma once
#include<string>

class HMQSession;

namespace cms
{
	class Message;
}

/****************begin of HMQMessage*******************/
class HMQMessage
{
	public:
		HMQMessage() = default;
		virtual ~HMQMessage();
		const std::string GetSenderID() const;
		const std::string GetReceiverID() const;
		void SetSenderID(const std::string& id);
		void SetReceiverID(const std::string& id);
		long long GetTimestamp() const;

		cms::Message* GetMessage();
		void SetMessage(cms::Message* msg);
	protected:
		cms::Message* m_message = nullptr;
};

inline cms::Message* HMQMessage::GetMessage()
{
	return m_message;
}

inline void HMQMessage::SetMessage(cms::Message* msg)
{
	m_message = msg;
}

/****************begin of HMQTextMessage*******************/
class HMQTextMessage:public HMQMessage
{
	public:
		HMQTextMessage() = default;
		HMQTextMessage(HMQSession* session);
		~HMQTextMessage() = default;
		
		std::string GetText();
		void SetText(const std::string& txt);
		void SetText(const char* txt);
};

/****************begin of HMQMapMessage*******************/
class HMQMapMessage: public HMQMessage
{
	public:
		HMQMapMessage() = default;
		HMQMapMessage(HMQSession* session);
		~HMQMapMessage() = default;
		std::string GetValue(const std::string& field);
		void SetValue(const std::string& field, const std::string& value);
};
