
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


// MySQL��ѯ���������ö�����ڣ���Ҫ��֤�����Ч
// ������Init����ʧ��,����ɾ���ö���
class PgSQLResult : public KCSQLResultBase
{
public:
	PgSQLResult(long affectRowCount);
	PgSQLResult(long errorCode, const char* errorMessage);
	PgSQLResult(PgSQLConnection* pConnection, PGresult * sqlResult);

	virtual ~PgSQLResult();

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

	PGresult*		m_sqlResult;
	int				m_sqlRow;		// ��ǰ��
};

#endif // KC_PG_SQL

#endif //__KC_PGSQL_H__

