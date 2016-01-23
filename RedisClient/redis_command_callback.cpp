/*************************************************************************
	> File Name: RedisClient/redis_command_callback.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 10时44分20秒
 ************************************************************************/
#include<iostream>
#include<list>

#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include "Utility/any_wrapper.h"

#include "redis_command_callback.h"
using namespace std;

RedisCommandCallback::RedisCommandCallback(function<void(any&, bool)>&& cb)
    :m_callback(cb)
{}

bool RedisCommandCallback::ReadReply(redisReply* reply, any& result)
{
	int ret = true;
	switch(reply->type)
	{
		case REDIS_REPLY_STATUS:
			{
				result = string(reply->str, reply->len);
			}
			break;
		case REDIS_REPLY_ERROR:
			{
				result = string(reply->str, reply->len);
				ret = false;
			}
			break;
		case REDIS_REPLY_INTEGER:
			{
				result = reply->integer;
			}
			break;
		case REDIS_REPLY_NIL:
			{

			}
			break;
		case REDIS_REPLY_STRING:
			{
				result = string(reply->str, reply->len);
			}
			break;
		case REDIS_REPLY_ARRAY:
			{
                result = list<any>(reply->elements);
				list<any>& lst = any_cast<list<any>&>(result);
                int i = 0;
                for(auto it: lst)
				{
					redisReply* tmp = reply->element[i++];
					ret = ReadReply(tmp, it);
                    if(!ret)
                    {
                        break;
                    }
				}
			}
			break;
	}

	return ret;
}

bool RedisCommandCallback::ParseReply(redisReply* reply, any& result)
{
	bool ret = true;
	ret = ReadReply(reply, result);
	freeReplyObject(reply);
	m_callback(result, ret);
	return ret;
}
