/*************************************************************************
	> File Name: HwSql/sql_connect.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月16日 星期三 10时37分07秒
 ************************************************************************/

#include<iostream>
#include<map>

using namespace std;
class SqlStmt;

class SqlConnect
{
	public:
		virtual bool Open(const char* path);
		virtual void Close();
		
		/*stmt*/
		virtual SqlStmt* Prepare(const char* sql);
		virtual void* GetHandler();
		void Remove(SqlStmt*);
		void Add(SqlStmt*);
	private:
		map<int, SqlStmt*> m_stmt_pool;
};
