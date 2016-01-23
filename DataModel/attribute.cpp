/**
 * @file DataModel/attribute.cpp
 * @brief 属性列表
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */

#include<iostream>
#include<vector>
#include <algorithm>

#include "Utility/string_tool.h"

#include "attribute.h"
using namespace std;

bool Attribute::parse(const string& info)
{
	if(info.empty())
	{
		return false;
	}

	vector<string> vecStr;
	Split(info, vecStr, ":");
	m_property_name = vecStr[0];
	if(vecStr.size() != 2)
	{
		return true;
	}

	vector<string> vecAttr;
	Split(vecStr[1], vecAttr, ",");
	if(vecAttr.size() == 0)
	{
		return true;
	}

	for(int i=0; i<vecAttr.size(); ++i)
	{
		vecStr.clear();
		Split(vecAttr[i], vecStr, "=");
		if(vecStr.size() == 1)
		{
			vecStr.push_back(string());
		}

		m_lst_node.push_back(AttributeNode(vecStr[0], vecStr[1]));
	}

	return true;
}

const string& Attribute::get(const string& name) const
{
	auto it = find_if(m_lst_node.begin(), m_lst_node.end(),
			[&](const AttributeNode& node)
			{
				return node.getKey() == name;
			});

	if(it != m_lst_node.end())
	{
		return it->getValue();
	}

	return AttributeNode::Empty;
}

bool Attribute::compare(const string& name, const string& value) const
{
	return get(name) == value;
}

bool Attribute::operator==(const Attribute& attr)
{
	return attr.m_property_name == this->m_property_name;
}

bool Attribute::operator==(const string& name)
{
	return name == m_property_name;
}
