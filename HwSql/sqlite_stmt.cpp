/*************************************************************************
	> File Name: HwSql/sqlite_stmt.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 16时13分22秒
 ************************************************************************/
#include "string.h"
#include<iostream>

#include "sqlite3.h"

#include "sqlite_stmt.h"
#include "sqlite_connect.h"
using namespace std;

SqliteStmt::SqliteStmt(SqliteConnect* connect)
{
	m_connect = connect;
}

int SqliteStmt::GetColumnType(int idx)
{
	int type = sqlite3_column_type(m_stmt, idx);
	if(type == SQLITE_INTEGER)
	{
		return StmtColumnType_Int;
	}
	else if(type == SQLITE_FLOAT)
	{
		return StmtColumnType_Double;
	}
	else if(type == SQLITE_BLOB)
	{
		return StmtColumnType_Blob;
	}
	else if(type == SQLITE_NULL)
	{
		return StmtColumnType_Null;
	}
}

int SqliteStmt::Bind(int idx, any&& any)
{
	if(any.type() == typeid(int))
	{
		BindInt(idx, any_cast<int>(any));
	}
	else if(any.type() == typeid(double))
	{
		BindDouble(idx, any_cast<double>(any));
	}
	else if(any.type() == typeid(string))
	{
		string str = any_cast<string>(any);
		BindBlob(idx, str.c_str(), str.length());
	}
	else if(any.type() == typeid(long))
	{
		BindInt64(idx, any_cast<long>(any));
	}
}

void SqliteStmt::Prepare(int id, const char* sql)
{
	m_id = id;
	sqlite3* handler = (sqlite3*)m_connect->GetHandler();
	sqlite3_prepare(handler, sql, strlen(sql), &m_stmt, NULL); 
	m_values.resize(sqlite3_column_count(m_stmt));
}

int SqliteStmt::BindInt(int idx, int value)
{
	int error = sqlite3_bind_int(m_stmt, idx, value);
	return GetBindError(error);
}

int SqliteStmt::BindDouble(int idx, double value)
{
	int error = sqlite3_bind_double(m_stmt, idx, value);
	return GetBindError(error);
}

int SqliteStmt::BindInt64(int idx, long value)
{
	int error = sqlite3_bind_int64(m_stmt, idx, value);
	return GetBindError(error);
}

int SqliteStmt::BindBlob(int idx, const void* value, long length)
{
	int error = sqlite3_bind_blob(m_stmt, idx, value, length, SQLITE_TRANSIENT);
	return GetBindError(error);
}

int SqliteStmt::Step()
{
	int ret = sqlite3_step(m_stmt);
	if(ret == SQLITE_BUSY)
	{
		return StmtStepRet_Busy;
	}
	else if(ret == SQLITE_DONE)
	{
		return StmtStepRet_Done;
	}
	else if(ret == SQLITE_ROW)
	{
		for(int i = 0; i<m_values.size(); ++i)
		{
			int type = GetColumnType(i);
			switch(type)
			{
				case StmtColumnType_Int:
					{
						m_values[i] = sqlite3_column_int(m_stmt, i);
					}
					break;
				case StmtColumnType_Double:
					{
						m_values[i] = sqlite3_column_double(m_stmt, i);
					}
					break;
				case StmtColumnType_Long:
					{
						m_values[i] = sqlite3_column_int64(m_stmt, i);
					}
					break;
				case StmtColumnType_Blob:
					{
						m_values[i] = string((const char*)sqlite3_column_text(m_stmt, i),
								sqlite3_column_bytes(m_stmt, i));
					}
					break;
				case StmtColumnType_Null:
					{
						m_values[i] = any();
					}
					break;
				default:
					{
						cout<<"Not support Type:"<<type<<endl;
					}
			}
		}
		return StmtStepRet_Row;
	}
	else
	{
		return StmtStepRet_Error;
	}
}

SqliteStmt::~SqliteStmt()
{
	Clear();
}

void SqliteStmt::Reset()
{
	sqlite3_clear_bindings(m_stmt);
}

void SqliteStmt::Clear()
{
	sqlite3_finalize(m_stmt);
}
