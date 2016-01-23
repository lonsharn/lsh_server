/**
 * @file sql_serializer.cpp
 * @brief class SqlSerializer
 * @author lonsharn
 * @version 1.0
 * @date 2015-11-06
 */

#include "sql_serializer.h"
#include "HalloSql/_kcsql.h"

#include<sstream>

#include "Utility/any_wrapper.h"

SqlSerializer::SqlSerializer(KCSQLConnection& conn, const list<Attribute*>& attr_lst, bool update)
    :m_conn(conn)
    ,m_attr_lst(attr_lst)
    ,m_bUpdate(update){}

bool SqlSerializer::tableExist(const std::string& table_name)
{
	KCSQLResult* result = NULL;

	std::stringstream ss;
	ss<<"select * from pg_tables where tablename='"<<table_name<<"'";
	result = m_conn.ExecuteQuery(ss.str().c_str());
	assert(result->IsSuccessed());

	if(result->GetNumRows() == 1)
	{
		return true;
	}
	return false;
}

/*
   bool SqlSerializer::createTable(const std::string& table_name)
   {
   std::string strQueryString;
   strQueryString += "create table " + table_name + "(";
   for(auto it: m_attr_lst)
   {
   if(it->get("sql_tn") != table_name)
   {
   continue;
   }

   std::string strKey;
   std::string strNotNull;
   std::string strUnique;
   if(it->get("sql_pk") != AttributeNode::Empty)
   {
   strKey = " primary key";
   }
   else
   {
   if(it->get("sql_nn") != AttributeNode::Empty)
   {
   strNotNull = " not null";
   }

   if(it->get("sql_u") != AttributeNode::Empty)
   {
   strUnique = " unique";
   }
   }

   std::string strFieldName = it->get("sql_fn");
   if(strFieldName == AttributeNode::Empty)
   {
   strFieldName = it->getPropertyName();
   }

   strQueryString += strFieldName + " " + it->get("sql_type") + strKey + strNotNull + strUnique + ",";
   }
   strQueryString.back() = ')';

   KCSQLResult* result = NULL;
   result = m_conn.ExecuteQuery(strQueryString.c_str());
   if(!result->IsSuccessed())
   {
   cout<<result->GetErrorMessage()<<endl;
   return false;
   }

   return true;
   }
   */

bool SqlSerializer::insertValue(std::string& tableName, std::list<std::pair<std::string, any> >& lst)
{
	std::string strQueryString;
	std::string strKeys;
	std::string strValues;
	for(auto it: m_attr_lst)
	{
		if(it->get("sql_tn") != tableName)
		{
			continue;
		}

		auto _it = std::find_if(lst.begin(), lst.end(),
				[&it](const std::pair<std::string, any>& p)
				{
				if(it->getPropertyName() == p.first)
				{
				return true;
				}

				return false;
				});

		if(_it == lst.end())
		{
			continue;
		}

		std::string strFieldName = it->get("sql_fn");
		if(strFieldName == AttributeNode::Empty)
		{
			strFieldName = it->getPropertyName();
		}
        strKeys += strFieldName + ",";

		std::string strFieldValue;
		if(_it->second.type() == typeid(std::string))
		{
			strFieldValue = "'" +  any_cast_string(_it->second) + "',";
		}
		else
		{
			strFieldValue = any_cast_string(_it->second) + ",";
		}
        strValues += strFieldValue;
	}

    strKeys.pop_back();
    strValues.pop_back();

	strQueryString = "insert into " + tableName + "(" + strKeys + ") ";
    strQueryString += "values(" + strValues + ")";

	KCSQLResult* result = NULL;
	result = m_conn.ExecuteQuery(strQueryString.c_str());
    cout<<strQueryString<<std::endl;
	if(!result->IsSuccessed())
	{
		cout<<result->GetErrorMessage()<<endl;
		return false;
	}

	return true;
}

bool SqlSerializer::updateValue(std::string& tableName, std::list<std::pair<std::string, any> >& lst)
{
	std::string strQueryString;
	std::string strKeyName;
	std::string strKeyValue;
	strQueryString = "update " + tableName + " set";
	for(auto it: m_attr_lst)
	{
		bool flag = false;
		if(it->get("sql_pk") != AttributeNode::Empty)
		{
			flag = true;
		}

		if(it->get("sql_tn") != tableName)
		{
			continue;
		}

		auto _it = std::find_if(lst.begin(), lst.end(),
				[&it](const std::pair<std::string, any>& p)
				{
				if(it->getPropertyName() == p.first)
				{
				return true;
				}

				return false;
				});

		if(_it == lst.end())
		{
			continue;
		}

		std::string strFieldName = it->get("sql_fn");
		if(strFieldName == AttributeNode::Empty)
		{
			strFieldName = it->getPropertyName();
		}

		std::string strFieldValue;
		if(_it->second.type() == typeid(std::string))
		{
			strFieldValue = "'" +  any_cast_string(_it->second) + "'";
		}
		else
		{
			strFieldValue = any_cast_string(_it->second);
		}

		if(flag)
		{
			strKeyName = strFieldName;
			strKeyValue = strFieldValue;
		}
        else
        {
		    strQueryString += " " + strFieldName + "=" + strFieldValue + ",";
        }
	}

	strQueryString.back() = ' ';

	strQueryString += "where " + strKeyName + " = " + strKeyValue;

	KCSQLResult* result = NULL;
	result = m_conn.ExecuteQuery(strQueryString.c_str());
    cout<<strQueryString<<std::endl;
	if(!result->IsSuccessed())
	{
		cout<<result->GetErrorMessage()<<endl;
		return false;
	}

	return true;
}

bool SqlSerializer::serialize(std::string& tableName, list<pair<std::string, any> >& lst)
{
	if(!tableExist(tableName))
	{
        std::cerr<<"table is not exist!"<<std::endl;
		return false;
	}

    if(m_bUpdate)
    {
	    return updateValue(tableName, lst);
    }
    else
    {
	    return insertValue(tableName, lst);
    }
}

bool SqlSerializer::deserialize(const std::string& tableName, std::list<std::pair<std::string, any> >& lst)
{
	return true;
}

bool SqlSerializer::selectValue(std::string& tableName, list<pair<std::string, any> >& lst)
{
	std::string strQueryString;
	std::stringstream ss(strQueryString);
	ss<<"select * from "<<tableName<<" where ";
	return true;
}
