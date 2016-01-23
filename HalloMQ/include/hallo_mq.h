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
};
