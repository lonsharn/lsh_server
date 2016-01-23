/**
 * @file attribute_node.cpp
 * @brief 类属性特征节点
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-23
 */
#pragma once
#include <string>

using namespace std;

class AttributeNode
{
	public:
		/**
		 * @brief 空的特性值
		 */
		static string Empty;

		/**
		 * @brief 特性节点的构造函数
		 *
		 * @param key 特性名
		 * @param value 特性值
		 */
		AttributeNode(const string& key, const string& value);

		/**
		 * @brief 析构函数
		 */
		~AttributeNode()=default;

		/**
		 * @brief 比较特性节点名和值
		 *
		 * @param key 比较的特性节点名
		 * @param value 比较的特性节点值
		 *
		 * @return 比较结果
		 */
		bool compare(const string& key, const string& value) const;

		/**
		 * @brief 获取特性节点的值
		 *
		 * @return 特性节点值
		 */
		const string& getValue() const;

		/**
		 * @brief 获取特性节点名
		 *
		 * @return 特性节点名
		 */
		const string& getKey() const;
	private:
		/**
		 * @brief 特性节点名
		 */
		string m_key;

		/**
		 * @brief 特性节点值
		 */
		string m_value;
};

inline const string& AttributeNode::getValue() const
{
	return m_value;
}

inline const string& AttributeNode::getKey() const
{
	return m_key;
}
