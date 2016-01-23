/**
 * @file DataModel/attribute_node.cpp
 * @brief 
 * @author lonsharn
 * @version 1.0
 * @date 2015-09-24
 */

#include "attribute_node.h"

string AttributeNode::Empty = "null";

AttributeNode::AttributeNode(const string& key, const string& value)
	:m_key(key)
	 ,m_value(value)
{
}

bool AttributeNode::compare(const string& key, const string& value) const
{
	return (key == m_key && value == m_value);
}
