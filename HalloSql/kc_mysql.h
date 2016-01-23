
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


// MySQL��ѯ���������ö�����ڣ���Ҫ��֤�����Ч
// ������Init����ʧ��,����ɾ���ö���
class MySQLResult : public KCSQLResultBase
{
public:
	MySQLResult(UINT64 iInsertID);

	MySQLResult(long affectRowCount);

	MySQLResult(long errorCode, const char* errorMessage);

	MySQLResult(MySQLConnection* pConnection, MYSQL_RES* sqlResult, MYSQL_FIELD* pField, bool bStoreResult);

	virtual ~MySQLResult();

	// ��ǰ�е�ĳһ�����Ƿ�Ϊ���ݿ������
	virtual bool IsNullField(long iFieldOffset);
	virtual bool IsNullField(const char* szFieldName);

	// ȡ��Ŀǰ��������
	virtual int Tell();
	// �ƶ���ǰ��Ϊ��һ�У�����������Ϊ0)
	virtual bool SeekToBegin();
	// ���õ�ǰ�е�������
	virtual bool Seek(int iRowOffset);
	// �ƶ���ǰ�е���һ��
	virtual bool Next();
	// �Ƿ������һ��
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

