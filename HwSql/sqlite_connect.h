/*************************************************************************
	> File Name: HwSql/sqlite_connect.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月16日 星期三 11时10分26秒
 ************************************************************************/

#include<iostream>

#include "sql_connect.h"
using namespace std;

struct sqlite3;
class SqliteConnect: public SqlConnect
{
	public:
		SqliteConnect() = default;
		bool Open(const char* path);
		void Close();
		SqlStmt* Prepare(const char* sql);
		void* GetHandler();
	private:
		sqlite3* m_handler = nullptr;
		int		m_startIndex = 10;
};
