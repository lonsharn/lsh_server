/*************************************************************************
	> File Name: RedisClient/redis_client.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月14日 星期一 15时40分50秒
 ************************************************************************/
#pragma once
#include<iostream>
#include<functional>

#include "Utility/any.h"
using namespace std;

struct event_base;
struct redisAsyncContext;

class RedisClient
{
	public:
		static RedisClient* Connect(const char* ip, short port);
		static void Disconnect(RedisClient* client);
		static void CommandCallback(redisAsyncContext *c, void *r, void *privdata);
		static void ConnectCallback(const redisAsyncContext *c, int status);
		static void DisconnectCallback(const redisAsyncContext *c, int status);
	public:
		RedisClient(redisAsyncContext* ctx)
			:m_ctx(ctx)
		{}
        ~RedisClient();
		void Command(const char* cmd, function<void(any&, bool)>&& callback);
		void StartAsync(event_base*);
	private:
		redisAsyncContext* m_ctx;
		event_base*			m_eb = nullptr;
		bool		m_ready = false;
};
