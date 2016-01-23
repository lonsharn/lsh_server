/**
 * @file property.h
 * @brief 属性集
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */
#pragma once

#include<iostream>
#include <algorithm>
#include <functional>

#include "serializer.h"
#include "property_node.h"

using namespace std;

template<typename C>
class Property
{
	public:
		Property();
		~Property() = default;
		
		/**
		 * @brief 初始化类型
		 */
		static void Init();

		/**
		 * @brief 设置一个属性值
		 *
		 * @tparam T 属性的类型
		 * @param name 属性名
		 * @param value 属性值
		 */
		template<typename T>
			bool setValue(const string& name, T&& value);

		/**
		 * @brief 获取一个属性的拷贝
		 *
		 * @tparam T 属性的类型
		 * @param name 属性名
		 *
		 * @return  属性值
		 */
		template<typename T>
			T getValue(const string&& name);

		/**
		 * @brief 为此类型的数据框架添加一个特性
		 *
		 * @param str 特性值，格式为 "PropertyName:AttributeName1=Value,AttributeName2=Value"
		 *
		 * @return  如果有存在的属性值，添加失败，返回false，否则返回true
		 */
		static bool addAttr(const string& str);

		list<Attribute*>& getAttribute();

		/**
		 * @brief 将Property的值序列化，包括json等格式
		 *
		 * @param result 序列化后的数据
		 * @param s 使用的序列化器
		 *
		 * @return 序列化结果
		 */
		bool Serialize(string& result, Serializer* s);

		/**
		 * @brief 将数据通过相应的序列化器序列化到property容器中
		 *
		 * @param str 序列化前的数据
		 * @param s 序列化器
		 *
		 * @return 序列化结果
		 */
		bool unSerialize(const string& str, Serializer* s);

		/**
		 * @brief 将property解析到list中
		 *
		 * @param lst 解析结果
		 */
		void get(list<pair<string, any> >& lst);

		/**
		 * @brief 从list数据结构读取property数据
		 *
		 * @param lst list数据
		 */
		void set(list<pair<string, any> >& lst);

		/**
		 * @brief 是否改动数据
		 *
		 * @return 返回true，数据已经改动；false，数据未曾改动
		 */
		bool isDirty();

		/**
		 * @brief 设置数据已经改动或者未改动
		 *
		 * @param flag 设置m_dirty的值
		 */
		void setDirty(bool flag);
	private:
		/**
		 * @brief property容器数据
		 */
		list<PropertyNode> m_list;

		/**
		 * @brief 是否已经改动
		 */
		bool m_dirty = false;

		/**
		 * @brief 属性的Attribute，同一属性名只对应一项Attribute
		 */
		static list<Attribute*> m_attribute_set;
};

template<typename C>
inline bool Property<C>::isDirty()
{
	return m_dirty;
}

template<typename C>
inline void Property<C>::setDirty(bool flag)
{
	m_dirty = flag;
}

template<typename C>
inline list<Attribute*>& Property<C>::getAttribute()
{
	return m_attribute_set;
}

template<typename C>
list<Attribute*> Property<C>::m_attribute_set;

template<typename C>
Property<C>::Property()
{
	for(auto it: m_attribute_set)
	{
		m_list.push_back(PropertyNode(*it));
	}
}

template<typename C>
	template<typename T>
bool Property<C>::setValue(const string& name, T&& value)
{
	auto it = find_if(m_list.begin(), m_list.end(), 
			[name](const PropertyNode& node)
			{
				if(node.getPropertyName() == name)
				{
					return true;
				}
				return false;
			});

	if(it == m_list.end())
	{
		return false;
	}

	it->template setValue<T>(value);
	return true;
}

template<typename C>
	template<typename T>
T Property<C>::getValue(const string&& name)
{
	auto it = find_if(m_list.begin(), m_list.end(), 
			[name](const PropertyNode& node)
			{
				if(node.getPropertyName() == name)
				{
					return true;
				}

				return false;
			});

	if(it != m_list.end())
	{
		return it->template getValue<T>();
	}

	return T();
}

template<typename C>
bool Property<C>::addAttr(const string& str)
{
	Attribute* attr = new Attribute();
	if(!attr->parse(str))
	{
		std::cerr<<"Parse Attribute Error:"<<str<<std::endl;
	}
	if(any_of(m_attribute_set.begin(), m_attribute_set.end(),
				[attr](const Attribute* _attr)
				{
					if(attr->getPropertyName() == _attr->getPropertyName())
					{
						return true;
					}

					return false;
				}))
	{
		return false;
	}
	else
	{
		m_attribute_set.push_back(attr);
		return true;
	}
}

template<typename C>
bool Property<C>::Serialize(string& out, Serializer* s)
{
	list<pair<string, any> > lst;
	get(lst);
	s->serialize(out, lst);
	return true;
}

template<typename C>
bool Property<C>::unSerialize(const string& in, Serializer* s)
{
	list<pair<string, any> > lst;
	s->deserialize(in, lst);
	set(lst);
	return true;
}

template<typename C>
void Property<C>::get(list<pair<string, any> >& ls)
{
	for(auto it: m_list)
	{
		any& a = it.get();
		ls.push_back(make_pair(it.getPropertyName(), a));
	}
}

template<typename C>
void Property<C>::set(list<pair<string, any> >& lst)
{
	std::for_each(m_list.begin(), m_list.end(),
			[&lst, this](PropertyNode& node)
			{
				auto it = find_if(lst.begin(), lst.end(),
						[&node](const pair<string, any>& v)
						{
							if(v.first == node.getPropertyName())
							{
								return true;
							}
							else
							{
								return false;
							}
						});
				if(it != lst.end())
				{
					node.set(it->second);
				}
			});
}
