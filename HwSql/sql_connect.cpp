/*************************************************************************
	> File Name: HwSql/sql_connect.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月16日 星期三 11时06分20秒
 ************************************************************************/
#include "sql_stmt.h"
#include "sql_connect.h"

void SqlConnect::Remove(SqlStmt* stmt)
{
	auto it = m_stmt_pool.find(stmt->GetID());
	if(it != m_stmt_pool.end())
	{
		delete it->second;
	}
	m_stmt_pool.erase(it);
}

void SqlConnect::Add(SqlStmt* stmt)
{
	m_stmt_pool.insert(make_pair(stmt->GetID(), stmt));
}
