/**
 * @file hmq_connect.h
 * @brief class HMQConnection
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */
#pragma once

#include<vector>
#include<map>
#include<atomic>
#include<thread>

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
		bool Init(const Json::Value& cfgValue);
		HMQSession* CreateSession(const std::string&& service, short type, long timeout = 0);
		void RemoveSession(HMQSession* session);

		const std::string& GetServiceName() const;
		const std::string& GetGroupName() const;
		cms::Connection* GetConnection();
		void OnException(const cms::CMSException& ex);
		void Close();
		bool Connect();
		void RetryConnect();
		void RecoverSession();
		bool IsStart();
		cms::Session* GetPooledSession();
	private:
		cms::Connection* m_connection = nullptr;
		std::string		m_group_name;
		std::string		m_service_name;
		std::string		m_broker_url;
		std::atomic<int>		m_retry_time;
		std::atomic<bool>		m_started;
		std::atomic<unsigned long long>		m_id;
		std::map<std::string, std::string>	m_dest_service;
		std::map<std::string, HMQSession*>	m_session_list;
		HMQExceptionListener* m_exception_listener = nullptr;
		std::thread*	m_retry_thread;
	private:
		std::vector<cms::Session*> m_session_pool;
};
inline bool HMQConnection::IsStart()
{
	return m_started;
}

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

