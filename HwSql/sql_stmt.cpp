/*************************************************************************
	> File Name: HwSql/sql_stmt.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 16时01分22秒
 ************************************************************************/

#include<iostream>
#include "sqlite_stmt.h"

using namespace std;
void SqlStmt::SetColumnValue(int field, any&& value)
{
	m_values[field] = std::move(value);
}

any& SqlStmt::GetColumnValue(int field)
{
	return m_values[field];
}

int SqlStmt::GetColumnCount()
{
	return m_values.size();
}

int SqlStmt::GetID()
{
	return m_id;
}
