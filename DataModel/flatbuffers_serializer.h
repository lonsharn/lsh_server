/**
 * @file flatbuffer_serializer.h
 * @brief flatbuffer序列化类
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-25
 */
#pragma once

#include<iostream>

#include "serializer.h"
using namespace std;

/**
 * @brief flatbuffer 序列化类
 */
class FlatbufferSerializer:public Serializer
{
	public:
		/**
		 * @brief constructor
		 */
		FlatbufferSerializer()=default;

		/**
		 * @brief desconstructor
		 */
		~FlatbufferSerializer()=default;

		/**
		 * @brief 将数据序列化为flatbuffer格式
		 *
		 * @param out 序列化后的flatbuffer数据
		 * @param lst 需要序列化的数据，pair列表，第一个为key，第二个为value
		 *
		 * @return	序列化结果
		 */
		bool serialize(string& out, list<pair<string, any> >& lst);

		/**
		 * @brief 将flatbuffer格式数据反序列化
		 *
		 * @param in	flatbuffer格式数据
		 * @param lst	反序列化后的结果 
		 *
		 * @return 反序列化结果
		 */
		bool deserialize(const string& in, list<pair<string, any> >& lst);
};
