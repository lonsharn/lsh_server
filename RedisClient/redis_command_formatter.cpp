/*************************************************************************
	> File Name: RedisClient/redis_command_formatter.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 11时05分46秒
 ************************************************************************/
#include <string.h>
#include<iostream>

#include "redis_command_formatter.h"
using namespace std;

void RedisCommandFormatter::ResetBuffer()
{
	memset(m_buffer, 0, MAX_COMMAND_SIZE);
}

//HashMap操作
const char* RedisCommandFormatter::HashMapInsert(const char* key, const char* field, const char* value)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "HSET %s %s %s", key, field, value);
	return m_buffer;
}

const char* RedisCommandFormatter::HashMapGet(const char* key, const char* field)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "HGET %s %s", key, field);
	return m_buffer;
}

const char* RedisCommandFormatter::HashMapGetAll(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "HGETALL %s", key);
	return m_buffer;
}

const char* RedisCommandFormatter::HashMapDelete(const char* key, const char* field)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "HDEL %s %s", key, field);
	return m_buffer;
}

const char* RedisCommandFormatter::HashMapLength(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "HLEN %s", key);
	return m_buffer;
}

//List操作
const char* RedisCommandFormatter::ListPush(const char* key, const char* value)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "LPUSH %s %s", key, value);
	return m_buffer;
}

const char* RedisCommandFormatter::ListPop(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "LPOP %s", key);
	return m_buffer;
}

const char* RedisCommandFormatter::ListLen(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "LLEN %s", key);
	return m_buffer;
}

const char* RedisCommandFormatter::ListRange(const char* key, size_t start, size_t end)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "LRANGE %s %ld %ld", key, start, end);
	return m_buffer;
}

const char* RedisCommandFormatter::StringSet(const char* key, const char* value)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "SET %s %s", key, value);
	return m_buffer;
}

const char* RedisCommandFormatter::StringGet(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "GET %s", key);
	return m_buffer;
}

const char* RedisCommandFormatter::StringLen(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "STRLEN %s", key);
	return m_buffer;
}

const char* RedisCommandFormatter::StringDel(const char* key)
{
	ResetBuffer();
	snprintf(m_buffer, MAX_COMMAND_SIZE - 1, "STRLEN %s", key);
	return m_buffer;
}
