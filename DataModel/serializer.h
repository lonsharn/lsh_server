/**
 * @file serializer.cpp
 * @brief 序列化器
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */
#pragma once

#include<iostream>
#include<list>

#include "Utility/any.h"
using namespace std;

class Serializer
{
	public:
		/**
		 * @brief 序列化接口
		 *
		 * @param data 序列化后的数据
		 * @param lst 序列化前的结构
		 *
		 * @return 序列化结构
		 */
		virtual bool serialize(string& data, list<pair<string, any> >& lst){return true;}

		/**
		 * @brief 反序列化接口
		 *
		 * @param data 反序列化前的数据
		 * @param lst 反序列化后的数据结构
		 *
		 * @return 序列化结果
		 */
		virtual bool deserialize(const string& data, list<pair<string, any> >& lst){return true;}

		virtual ~Serializer() = default;
};

