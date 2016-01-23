/*************************************************************************
	> File Name: RedisClient/redis_client.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月14日 星期一 15时40分47秒
 ************************************************************************/
#include <stdio.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>

#include "Utility/log4cpp_wrapper.h"

#include "redis_client.h"
#include "redis_command_callback.h"
using namespace std;

RedisClient::~RedisClient()
{
    if(m_eb)
    {
        event_base_loopbreak(m_eb);
        event_base_free(m_eb);
        m_eb = nullptr;
    }

    if(m_ctx)
    {
	    redisAsyncFree(m_ctx);
        m_ctx = nullptr;
    }
}

RedisClient* RedisClient::Connect(const char* ip, short port)
{
	redisAsyncContext *c = redisAsyncConnect(ip, port);
	if (c->err)
	{
		/* Let *c leak for now... */
		printf("Error: %s\n", c->errstr);
		return nullptr;
	}
	
	RedisClient* client = new RedisClient(c);
	c->data = (void*)client;
	return client;
}

void RedisClient::Disconnect(RedisClient* client)
{
	redisAsyncDisconnect(client->m_ctx);
}

void RedisClient::CommandCallback(redisAsyncContext* c, void* r, void* privdata)
{
	redisReply *reply = (redisReply*)r;
	if (reply == NULL)
	{
		return;
	}

	RedisCommandCallback* cb = (RedisCommandCallback*)privdata;
	any result;
	cb->ParseReply(reply, result);
    freeReplyObject(reply);
	delete cb;
}

void RedisClient::ConnectCallback(const redisAsyncContext *c, int status)
{
	RedisClient* client = (RedisClient*)c->data;
	if(status == 0)
	{
		client->m_ready = true;
	}
	else
	{
		client->m_ready = false;
	}
}

void RedisClient::DisconnectCallback(const redisAsyncContext *c, int status)
{
	RedisClient* client = (RedisClient*)c->data;
    cout<<"DisconnectCallback"<<endl;
	client->m_ready = false;
	if(status == REDIS_ERR)
	{
        //cout<<"Error Disconnect:"<<c->errstr<<endl;
	}
	else
	{
        delete client;
		//cout<<"Disconnect completed!"<<endl;
	}
}

void RedisClient::StartAsync(event_base* eb)
{
    bool flag = false;
	if(eb == nullptr)
	{
        flag = true;
	}

    if(flag)
    {
		eb = event_base_new();
		m_eb = eb;
    }

	redisLibeventAttach(m_ctx, eb);
	redisAsyncSetConnectCallback(m_ctx, &RedisClient::ConnectCallback);
	redisAsyncSetDisconnectCallback(m_ctx, &RedisClient::DisconnectCallback);
    if(flag)
    {
	    event_base_dispatch(eb);
    }
}

void RedisClient::Command(const char* cmd, function<void(any&, bool)>&& callback)
{
	RedisCommandCallback* cb = new RedisCommandCallback(std::move(callback));
	redisAsyncCommand(m_ctx, RedisClient::CommandCallback, cb, cmd);
}
