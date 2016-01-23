
#ifndef __KC_MYSQL_H__
#define __KC_MYSQL_H__

#include "_kcsql.h"

#ifdef KC_MY_SQL

#include "kc_sqlbase.h"

#ifdef _WIN32
#include <windows.h>
#pragma comment( lib, "libmysql.lib" )
#else
//#include <sys/socket.h>
#endif

#include <mysql.h>


class MySQLResult;

class MySQLConnection : public KCSQLConnectionBase
{
public:
	MySQLConnection();
	virtual ~MySQLConnection();

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

	MYSQL*		m_conn;

	friend class MySQLResult;
};


// MySQL查询结果，如果该对象存在，则要保证结果有效
// 即调用Init函数失败,必须删除该对象
class MySQLResult : public KCSQLResultBase
{
public:
	MySQLResult(UINT64 iInsertID);

	MySQLResult(long affectRowCount);

	MySQLResult(long errorCode, const char* errorMessage);

	MySQLResult(MySQLConnection* pConnection, MYSQL_RES* sqlResult, MYSQL_FIELD* pField, bool bStoreResult);

	virtual ~MySQLResult();

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

	int				m_iCurrentRow;

	unsigned long*	m_sqlRowLengths;

	/// query result for SELECT type queries, NULL otherwise
	MYSQL_RES*		m_sqlResult;
	/// the most recent row returned by fetch operation
	MYSQL_ROW		m_sqlRow;

	MYSQL_FIELD*	m_pField;
};

#endif // KC_MY_SQL

#endif //__KC_MYSQL_H__

