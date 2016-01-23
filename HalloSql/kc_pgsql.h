
#ifndef __KC_PGSQL_H__
#define __KC_PGSQL_H__

#include "_kcsql.h"

#ifdef KC_PG_SQL

#include "kc_sqlbase.h"


#include <libpq-fe.h>

#ifdef _WIN32
#pragma comment( lib, "libpq.lib" )
#endif

class PgSQLResult;

class PgSQLConnection : public KCSQLConnectionBase
{
public:
	PgSQLConnection();
	virtual ~PgSQLConnection();

	virtual bool Init(
		const char* host,
		const char* database,
		const char* username,
		const char* password,
		unsigned short port
		);

	virtual KCSQLResult* ExecuteQuery(
		const char* queryStr,
		long iTimeout
		);

	virtual KCSQLResult* ExecuteQueryEx(
		const char* queryStr,
		long iTimeout,
		int type
		);

	virtual bool Connect(int iTimeout);
	virtual void Disconnect();
	virtual bool IsConnected();

protected:

	PGconn*		m_conn;

	friend class PgSQLResult;
};


// MySQL查询结果，如果该对象存在，则要保证结果有效
// 即调用Init函数失败,必须删除该对象
class PgSQLResult : public KCSQLResultBase
{
public:
	PgSQLResult(long affectRowCount);
	PgSQLResult(long errorCode, const char* errorMessage);
	PgSQLResult(PgSQLConnection* pConnection, PGresult * sqlResult);

	virtual ~PgSQLResult();

	// 当前行的某一属性是否为数据库空属性
	virtual bool IsNullField(long iFieldOffset);
	virtual bool IsNullField(const char* szFieldName);

	// 取得目前的行索引
	virtual int Tell();
	// 移动当前行为第一行（设置行索引为0)
	virtual bool SeekToBegin();
	// 设置当前行的行索引
	virtual bool Seek(int iRowOffset);
	// 移动当前行到下一行
	virtual bool Next();
	// 是否到了最后一行
	virtual bool IsEnd();

protected:

	PGresult*		m_sqlResult;
	int				m_sqlRow;		// 当前行
};

#endif // KC_PG_SQL

#endif //__KC_PGSQL_H__

