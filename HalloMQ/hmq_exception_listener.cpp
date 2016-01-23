/**
 * @file hmq_exception_listener.cpp
 * @brief 异常监听器
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-16
 */
#include"hmq_exception_listener.h"

HMQExceptionListener::HMQExceptionListener(std::function<void(const cms::CMSException&)>&& handler)
{
	m_handler = handler;
}

void HMQExceptionListener::onException(const cms::CMSException& ex)
{
	m_handler(ex);
}

void HMQExceptionListener::SetExceptionListener(std::function<void(const cms::CMSException&)>&& handler)
{
	m_handler = handler;
}
