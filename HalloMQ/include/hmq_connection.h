/**
 * @file hmq_connect.h
 * @brief class HMQConnection
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#pragma once

#include<list>
#include<map>
#include<atomic>

namespace Json
{
	class Value;
}

namespace cms
{
	class Connection;
	class ConnectionFactory;
	class CMSException;
	class Session;
}

class HMQSession;
class HMQExceptionListener;

class HMQConnection
{
	public:
		HMQConnection() = default;
		~HMQConnection();
		HMQSession* CreateSession(const std::string&& service, short type, long timeout = 0);
		void RemoveSession(HMQSession* session);
		void Close();
};

inline const std::string& HMQConnection::GetServiceName() const
{
	return m_service_name;
}

inline const std::string& HMQConnection::GetGroupName() const
{
	return m_group_name;
}

inline cms::Connection* HMQConnection::GetConnection()
{
	return m_connection;
}

inline cms::Session* HMQConnection::GetDefaultSession()
{
	return m_default_session;
}
