/**
 * @file hallo_mq.cpp
 * @brief class HalloMQ
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-21
 */

#include "hallo_mq.h"
#include "hmq_connection.h"
#include <fstream>
#include <functional>
#include <log4cpp/Category.hh>
#include <activemq/library/ActiveMQCPP.h>
HalloMQ* HalloMQ::m_instance = new HalloMQ;
bool HalloMQ::InitByJsonString(const std::string& path)
{
	activemq::library::ActiveMQCPP::initializeLibrary();
	std::ifstream ifs;
	ifs.open(path.c_str());
	assert(ifs.is_open());
	Json::Reader reader;
	if(!reader.parse(ifs, m_json_cfg))
	{
		log4cpp::Category::getInstance("HalloMQ").error("Read json file:%s failed!,%s", path.c_str(), reader.getFormattedErrorMessages().c_str());
		ifs.close();
		return false;
	}
	ifs.close();
	m_host_name = m_json_cfg["host-name"].asString();
	for(auto it: m_json_cfg["brokers"])
	{
		HMQConnection* conn = Conn(it);
		if(!conn)
		{
			continue;
		}
		m_connection_list.insert(std::make_pair(conn->GetGroupName(), conn));
	}
	return true;
}

HMQConnection* HalloMQ::getConn(const std::string& brokerName)
{
	auto it = m_connection_list.find(brokerName);
	if(it == m_connection_list.end())
	{
		return nullptr;
	}
	return it->second;
}

HMQConnection* HalloMQ::Conn(const Json::Value& cfg)
{
	HMQConnection* conn = new HMQConnection;
	if(conn->Init(cfg))
	{
		return conn;
	}
	else
	{
		delete conn;
		return nullptr;
	}
}

HalloMQ* HalloMQ::getInstance()
{
	return m_instance;
}

void HalloMQ::delInstance()
{
	if(m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

HalloMQ::~HalloMQ()
{
	activemq::library::ActiveMQCPP::shutdownLibrary();
}

HalloMQ::HalloMQ()
{
	activemq::library::ActiveMQCPP::initializeLibrary();
}

void HalloMQ::RemoveConnection(HMQConnection* conn)
{
	m_connection_list.erase(conn->GetGroupName());
}
