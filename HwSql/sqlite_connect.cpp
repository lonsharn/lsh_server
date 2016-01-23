/*************************************************************************
	> File Name: HwSql/sqlite_connect.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月16日 星期三 11时10分07秒
 ************************************************************************/

#include<iostream>

#include "sqlite3.h"

#include "sqlite_stmt.h"
#include "sqlite_connect.h"

using namespace std;


bool SqliteConnect::Open(const char* path)
{
	if(sqlite3_open(path, &m_handler) == 0)
	{
		return true;
	}

	return false;
}

void SqliteConnect::Close()
{
	sqlite3_close(m_handler);
}

SqlStmt* SqliteConnect::Prepare(const char* sql)
{
	SqlStmt* stmt = new SqliteStmt(this);
	stmt->Prepare(m_startIndex++, sql);
	Add(stmt);
	return stmt;
}

void* SqliteConnect::GetHandler()
{
	return (void*)m_handler;
}
