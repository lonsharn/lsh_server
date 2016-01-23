/**
 * @file DataModel/attribute.h
 * @brief 特性列表
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */
#pragma once
#include "attribute_node.h"

#include<list>
using namespace std;

class Attribute
{
	public:
		/**
		 * @brief 解析字符串成特性结构
		 *
		 * @param info 特性的字符串结构(PropertyName:AttributeName1=value,AttributeName2=value)，不要留有空格符
		 */
		Attribute() = default;

		/**
		 * @brief 获取某项特性节点的值
		 *
		 * @param name 特性名
		 *
		 * @return 特性值
		 */
		const string& get(const string& name) const;
		
		/**
		 * @brief 比较特性值
		 *
		 * @param name 特性名
		 * @param value 特性值
		 *
		 * @return 比较结果（字符串比较)
		 */
		bool compare(const string& name, const string& value) const;

		/**
		 * @brief 获取属性名
		 *
		 * @return 返回属性名
		 */
		const string& getPropertyName() const;

		/**
		 * @brief 解析字符串成特性结构
		 *
		 * @param info 特性的字符串结构(PropertyName:AttributeName1=value,AttributeName2=value)，不要留有空格符
		 *
		 * @return 解析结果
		 */
		bool parse(const string& info);

		/**
		 * @brief 比较两项特性表的属性名是否一致
		 *
		 * @param attr 另外的一个特性表
		 *
		 * @return 比较结果
		 */
		bool operator==(const Attribute& attr);

		/**
		 * @brief 比较特性表的属性名
		 *
		 * @param name 特性名
		 *
		 * @return 比较结果
		 */
		bool operator==(const string& name);

		/**
		 * @brief 析构函数
		 */
		~Attribute() = default;
	private:
		/**
		 * @brief 特性表
		 */
		list<AttributeNode> m_lst_node;

		/**
		 * @brief 属性名
		 */
		string m_property_name;
};

inline const string& Attribute::getPropertyName() const
{
	return m_property_name;
}
