/**
 * @file hmq_message.cpp
 * @brief class HMQMessage
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#include "hmq_message.h"
#include "hmq_session.h"
#include <cms/Message.h>
#include <cms/TextMessage.h>
#include <cms/MapMessage.h>
#include <cms/Session.h>
using namespace std;
/*****************Begin Of HMQMessage*****************/
HMQMessage::~HMQMessage()
{
	if(m_message)
	{
		delete m_message;
		m_message = nullptr;
	}
}
const string HMQMessage::GetSenderID() const
{
	return m_message?m_message->getStringProperty("HalloSender"):nullptr;
}
const string HMQMessage::GetReceiverID() const
{
	return m_message?m_message->getCMSCorrelationID():nullptr;
}
void HMQMessage::SetSenderID(const string& id)
{
	if(m_message)
	{
		m_message->setStringProperty("HalloSender", id);
	}
}
void HMQMessage::SetReceiverID(const string& id)
{
	if(m_message)
	{
		m_message->setCMSCorrelationID(id);
	}
}
long long HMQMessage::GetTimestamp() const
{
	return m_message?m_message->getCMSTimestamp():0;
}
/*****************Begin Of HMQTextMessage************/
HMQTextMessage::HMQTextMessage(HMQSession* session)
{
	m_message = session->GetSession()->createTextMessage();
}
string HMQTextMessage::GetText()
{
	if(m_message == nullptr)
	{
		return string();
	}
	return static_cast<cms::TextMessage*>(m_message)->getText();
}
void HMQTextMessage::SetText(const string& txt)
{
	if(m_message)
	{
		static_cast<cms::TextMessage*>(m_message)->setText(txt);
	}
}
void HMQTextMessage::SetText(const char* txt)
{
	if(m_message)
	{
		static_cast<cms::TextMessage*>(m_message)->setText(txt);
	}
}
/*****************Begin Of HMQMapMessage*************/
HMQMapMessage::HMQMapMessage(HMQSession* session)
{
	m_message = session->GetSession()->createMapMessage();
}
string HMQMapMessage::GetValue(const string& field)
{
	if(m_message == nullptr)
	{
		return string();
	}
	return static_cast<cms::MapMessage*>(m_message)->getString(field);
}
void HMQMapMessage::SetValue(const string& field, const string& value)
{
	if(m_message)
	{
		static_cast<cms::MapMessage*>(m_message)->setString(field, value);
	}
}
