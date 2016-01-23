/**
 * @file json_serializer.h
 * @brief json序列化类
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-25
 */
#pragma once

#include<iostream>

#include <json/json.h>

#include "serializer.h"
using namespace std;

/**
 * @brief json 序列化类
 */
class JsonSerializer:public Serializer
{
	public:
		/**
		 * @brief constructor
		 */
		JsonSerializer()=default;

		/**
		 * @brief desconstructor
		 */
		~JsonSerializer()=default;

		/**
		 * @brief 将数据序列化为json格式
		 *
		 * @param out 序列化后的json数据
		 * @param lst 需要序列化的数据，pair列表，第一个为key，第二个为value
		 *
		 * @return	序列化结果
		 */
		bool serialize(string& out, list<pair<string, any> >& lst);

		/**
		 * @brief 将json格式数据反序列化
		 *
		 * @param in	json格式数据
		 * @param lst	反序列化后的结果 
		 *
		 * @return 反序列化结果
		 */
		bool deserialize(const string& in, list<pair<string, any> >& lst);

		/**
		 * @brief 将av中的值读取到json数据结构中去 ，支持的复合类型为
		 * list<any>，list<pair<string, any> >;
		 *
		 * @param jv json数据格式
		 * @param av any数据格式
		 */
		static void recurseRead(Json::Value& jv, any& av);

		/**
		 * @brief 将json数据中的值读取到any中 ，支持的复合类型为
		 * list<any>，list<pair<string, any> >;
		 *
		 * @param jv json数据格式
		 * @param av any数据格式
		 */
		static void recurseWrite(Json::Value& jv, any& av);
};
