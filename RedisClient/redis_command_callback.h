/*************************************************************************
	> File Name: RedisClient/redis_command_callback.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 10时42分53秒
 ************************************************************************/
#pragma once

#include<iostream>
#include<functional>
using namespace std;

class RedisCommandCallback
{
	public:
		RedisCommandCallback(function<void(any&, bool)>&&);
		bool ParseReply(redisReply*, any&);
		bool ReadReply(redisReply*, any&);
	private:
		function<void(any&, bool)> m_callback;
};
