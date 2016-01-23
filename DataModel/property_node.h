/**
 * @file DataModel/property_node.h
 * @brief 属性节点
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */
#pragma once

#include<iostream>

#include "Utility/any.h"
#include "attribute.h"

using namespace std;

class PropertyNode
{
	public:
		/**
		 * @brief 构造一个Property节点
		 *
		 * @param attr 特性值
		 */
		PropertyNode(const Attribute& attr);

		/**
		 * @brief 构造一个Property节点
		 *
		 * @param node property节点
		 */
		PropertyNode(const PropertyNode& node);

		/**
		 * @brief 获取属性名
		 *
		 * @return 返回属性名
		 */
		const string& getPropertyName() const;

		/**
		 * @brief 获取节点的值
		 *
		 * @tparam T 获取的值的类型
		 *
		 * @return 返回值
		 */
		template<typename T>
			T getValue();

		/**
		 * @brief 
		 *
		 * @tparam T
		 * @param value
		 */
		template<typename T>
			void setValue(T& value);

		/**
		 * @brief 
		 *
		 * @return 
		 */
		any& get();

		/**
		 * @brief 
		 *
		 * @param any
		 */
		void set(any&);

		/**
		 * @brief 根据特性名获取节点的某项特性
		 *
		 * @param name 特性名
		 *
		 * @return 特性的值
		 */
		const string& getAttribute(const string& name)const;

		/**
		 * @brief 比较特性名的特性值是否一致
		 *
		 * @param name 特性名
		 * @param value 特性值
		 *
		 * @return 比较结果
		 */
		bool compare(const string& name, const string& value)const;
	private:
		/**
		 * @brief 特性列表
		 */
		const Attribute& m_attribute;

		/**
		 * @brief 属性值
		 */
		any m_property_value;
};

inline const string& PropertyNode::getPropertyName() const
{
	return m_attribute.getPropertyName();
}

inline any& PropertyNode::get()
{
	return m_property_value;
}
inline void PropertyNode::set(any& value)
{
	m_property_value = std::move(value);
}

template<typename T>
T PropertyNode::getValue()
{
	return any_cast<T>(m_property_value);
}

template<typename T>
void PropertyNode::setValue(T& value)
{
	m_property_value = any(value);
}
