/*************************************************************************
	> File Name: HwSql/sqlite_stmt.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 16时13分24秒
 ************************************************************************/
#pragma once

#include<iostream>

#include "sql_stmt.h"
using namespace std;

struct sqlite3_stmt;
class SqliteConnect;
class SqliteStmt:public SqlStmt
{
	public:
		SqliteStmt(SqliteConnect* connect);
		~SqliteStmt();
		void Prepare(int id, const char* sql);
		int Step();
		void Reset();
		void Clear();
		int Bind(int idx, any&& any);
		int GetColumnType(int idx);
	private:
		int BindInt(int idx, int value);
		int BindDouble(int idx, double value);
		int BindInt64(int idx, long value);
		int BindBlob(int idx, const void* value, long length);

		int GetBindError(int error);
		sqlite3_stmt* m_stmt = nullptr;
		SqliteConnect* m_connect = nullptr;
};
