/**
 * @file hallo_mq.h
 * @brief class HalloMQ
 * @author lonsharn
 * @version 1.0
 * @date 2015-10-15
 */

#include<map>

#include <json/json.h>

class HMQConnection;
class HalloMQ
{
	public:
		bool InitByJsonString(const std::string& path);
		HMQConnection* getConn(const std::string& brokerName);
		void RemoveConnection(HMQConnection* conn);
		static HalloMQ* getInstance();
		static void delInstance();
	private:
		~HalloMQ();
		HalloMQ();
		HMQConnection* Conn(const Json::Value& cfg);
	private:
		std::map<std::string, HMQConnection*> m_connection_list;
		static HalloMQ* m_instance;
		std::string m_host_name;
		Json::Value	m_json_cfg;
};
