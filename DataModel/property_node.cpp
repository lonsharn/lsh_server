/**
 * @file DataModel/property_node.cpp
 * @brief 属性节点
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */

#include<iostream>

#include "property_node.h"
using namespace std;

PropertyNode::PropertyNode(const Attribute& attr)
	:m_attribute(attr)
{
}

PropertyNode::PropertyNode(const PropertyNode& node)
	:m_attribute(node.m_attribute)
	 ,m_property_value(node.m_property_value)
{
}

const string& PropertyNode::getAttribute(const string& name)const
{
	return m_attribute.get(name);
}

bool PropertyNode::compare(const string& name, const string& value)const
{
	return m_attribute.compare(name, value);
}
