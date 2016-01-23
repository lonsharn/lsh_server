/**
 * @file hmq_exception_listener.h
 * @brief 异常监听器
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-16
 */
#pragma once

#include<functional>
#include<cms/ExceptionListener.h>

class HMQExceptionListener: public cms::ExceptionListener
{
	public:
		HMQExceptionListener() = default;
		HMQExceptionListener(std::function<void(const cms::CMSException&)>&& handler);
		void onException(const cms::CMSException& ex);
		void SetExceptionListener(std::function<void(const cms::CMSException&)>&& handler);
	public:
		std::function<void(const cms::CMSException&)> m_handler;
};
