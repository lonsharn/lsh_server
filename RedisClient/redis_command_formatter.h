/*************************************************************************
	> File Name: RedisClient/redis_command_formatter.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 10时54分34秒
 ************************************************************************/
#pragma once

#include<iostream>
using namespace std;

#define MAX_COMMAND_SIZE 4096
class RedisCommandFormatter
{
	public:
		RedisCommandFormatter() = default;
		~RedisCommandFormatter() = default;

		//HashMap操作
		const char* HashMapInsert(const char* key, const char* field, const char* value);
		const char* HashMapDelete(const char* key, const char* field);
		const char* HashMapLength(const char* key);
		const char* HashMapGet(const char* key, const char* field);
		const char* HashMapGetAll(const char* key);

		//List操作
		const char* ListPush(const char* key, const char* value);
		const char* ListPop(const char* key);
		const char* ListLen(const char* key);
		const char* ListRange(const char* key, size_t start, size_t end);

		//string操作
		const char* StringSet(const char* key, const char* value);
		const char* StringGet(const char* key);
		const char* StringLen(const char* key);
		const char* StringDel(const char* key);
	private:
		char m_buffer[MAX_COMMAND_SIZE] = {0};
		void ResetBuffer();
};
