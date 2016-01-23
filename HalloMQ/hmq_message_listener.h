/**
 * @file hmq_message_listener.h
 * @brief 消息监听器
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-16
 */
#pragma once

#include<functional>

#include <cms/MessageListener.h>

namespace cms
{
	class Message;
}

class HMQMessage;

class HMQMessageListener: public cms::MessageListener
{
	public:
		HMQMessageListener() = default;
		HMQMessageListener(std::function<void(const HMQMessage*)>&& handler);
		virtual void onMessage(const cms::Message* msg);
		void SetMessageListener(std::function<void(const HMQMessage*)>&& handler);
	public:
		std::function<void(const HMQMessage*)> m_handler;
};
