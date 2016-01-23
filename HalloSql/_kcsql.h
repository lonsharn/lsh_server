

#ifndef __KC_SQL_H__
#define __KC_SQL_H__

// ʹ�õĿ⿪��
// WIN32
// ��project������
// KC_MY_SQL
// KC_PG_SQL

#ifndef SQL_API
#ifdef _WIN32
#define SQL_API __declspec(dllexport)
#define SQL_API_EXPORT __declspec(dllexport)
#define SQL_API_IMPORT __declspec(dllimport)
#else
#define SQL_API
#define SQL_API_EXPORT
#define SQL_API_IMPORT
#endif
#endif

#include <list>
#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <iostream>

// ExecuteQueryEx type �Ŀ�ѡ�ֶ�
// һ�δӷ�����������������
#define EXEC_SQL_STORE_RESULT	0
// ȡ���һ��һ���Ĵӷ��������� // ��������ʱʹ�����ѡ��
#define EXEC_SQL_FETCH_RESULT	1

#ifdef _WIN32
#ifndef KCSQL_LIB
#pragma comment(lib, "kcsql.lib")
#endif
#endif

using namespace std;

#if !defined(UINT64)
typedef unsigned long long UINT64;
#endif

#ifdef KC_MY_SQL
SQL_API int SQLEscapeString(char *to, const char *from, unsigned int length);
SQL_API int SQLEscapeString(string& szSql);
SQL_API int SQLEscapeString(string& to, const string& szSql);
#endif

class KCSQLConnection;

class SQL_API KCSQLResult
{
public:
	virtual ~KCSQLResult() {}

	virtual long GetNumRows() const = 0;
	virtual long GetNumFields() const = 0;

	virtual bool IsNullField(long iFieldOffset) = 0;
	virtual bool IsNullField(const char* szFieldName) = 0;

	virtual int Tell() = 0;
	virtual bool SeekToBegin() = 0;
	virtual bool Seek(int iRowOffset) = 0;

	virtual bool Next() = 0;
	virtual bool IsEnd() = 0;

	virtual bool IsFieldExist(const char* szFieldName) = 0;

	virtual const char* GetField(int iFieldOffset) = 0;
	virtual const char* GetField(const char* szFieldName) = 0;

	virtual const char* GetFieldName(int iFieldOffset) = 0;

	virtual const char* operator [](int iFieldOffset) = 0;
	virtual const char* operator [](const char* szFieldName) = 0;

	virtual bool IsSuccessed() = 0;
	virtual long GetErrorCode() = 0;
	virtual const char* GetErrorMessage() = 0;

	virtual long GetAffectRowCount() = 0;
	virtual UINT64 GetInsertID() = 0;

	// ��ӡ��ǰ��
	virtual void PrintRow() = 0;
};


class KCSQLConnectionBase;

// ������
// ����������������
class SQL_API KCSQLConnection
{
public:

	KCSQLConnection();

	~KCSQLConnection();

	bool Create(
		const char* szSQLType,	// ���ݿ�����
		const char* host,
		const char* database,
		const char* username,
		const char* password,
		unsigned short port
		);

	KCSQLResult* ExecuteQuery(
		const char* queryStr,
		long timeout = -1
		);

	KCSQLResult* ExecuteQueryEx(
		const char* queryStr,
		long timeout = -1,
		int	type = EXEC_SQL_STORE_RESULT
		);

	bool Connect(int iTimeout = 60000);
	void Disconnect();
	bool IsConnected();

	const char* GetName();
	void SetName(const char* szName);

	ostream& Error();

	ostream& Log();

	ostream& Console();

	// �����������
	// ������ǰ����ָ��
	// ������ "error" "log" "console"
	ostream* SetStream(const char* szStreamName, ostream* pStream);

	// ���������Զ�����
	// iAutoConnect����0��ʾ�Զ�����
	void EnableAutoConnect(int iAutoConnect);

protected:

	KCSQLConnectionBase*	m_This;			// ����Ķ���ָ��
};


/*
KCSQLConnection* CreateSQLConnection(
							const char* szSQLType,	// ���ݿ�����
							const char* host,
							const char* database,
							const char* username,
							const char* password = NULL,
							unsigned short port = 0
							);
*/
#endif // __KC_SQL_H__



