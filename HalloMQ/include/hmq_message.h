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
};

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
