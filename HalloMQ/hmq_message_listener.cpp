/**
 * @file hmq_message_listener.cpp
 * @brief 消息监听器
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-16
 */
#include"hmq_message_listener.h"
#include"hmq_message.h"

#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>

HMQMessageListener::HMQMessageListener(std::function<void(const HMQMessage*)>&& handler)
{
	m_handler = handler;
}

void HMQMessageListener::onMessage(const cms::Message* msg)
{
	cms::Message* _msg = const_cast<cms::Message*>(msg);
	HMQMessage* hmsg = nullptr;
	if(dynamic_cast<cms::TextMessage*>(_msg))
	{
		hmsg = new HMQTextMessage();
	}
	else if(dynamic_cast<cms::MapMessage*>(_msg))
	{
		hmsg = new HMQMapMessage();
	}
	else
	{
	}

	hmsg->SetMessage(const_cast<cms::Message*>(_msg));
	m_handler(hmsg);
}

void HMQMessageListener::SetMessageListener(std::function<void(const HMQMessage*)>&& handler)
{
	m_handler = handler;
}
