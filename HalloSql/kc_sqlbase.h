
#ifndef __KC_SQL_BASE_H__
#define __KC_SQL_BASE_H__

#include "_kcsql.h"
#include "kc_string.h"

#include "CMutex.h"
#include <string.h>

class KCSQLConnectionBase
{
public:

	KCSQLConnectionBase()
	{
		m_pStreamError = NULL;
		m_pStreamLog = NULL;
		m_pStreamConsole = NULL;

		m_errorCode = 0;

		m_iAutoConnect = 1;

		m_lRef = 1;
	}

	virtual ~KCSQLConnectionBase()
	{
		if (m_lRef!=0)
		{
			assert("KCSQLConnectionBase���ͷţ������ü�����Ϊ0"==0);
		}
	}

	virtual bool Init(
		const char* host,
		const char* database,
		const char* username,
		const char* password = NULL,
		unsigned short port = 0
		) = 0;

	virtual KCSQLResult* ExecuteQuery(
		const char* queryStr,
		long timeout = -1
		) = 0;

	virtual KCSQLResult* ExecuteQueryEx(
		const char* queryStr,
		long timeout = -1,
		int	type = 0
		) = 0;

	virtual bool Connect(int iTimeout = -1) = 0;
	virtual void Disconnect() = 0;
	virtual bool IsConnected() = 0;

	const char* GetName() { return m_szName.c_str(); }
	void SetName(const char* szName) { m_szName = szName; }

	long AddRef() { return ++m_lRef; };
	void Release() { --m_lRef; if (m_lRef==0) delete this; }

	ostream& Error()
	{
		if (m_pStreamError)
		{
			return	*m_pStreamError;
		}
		return	cerr;
	}

	ostream& Log()
	{
		if (m_pStreamLog)
		{
			return	*m_pStreamLog;
		}
		return	cout;
	}

	ostream& Console()
	{
		if (m_pStreamConsole)
		{
			return	*m_pStreamConsole;
		}
		return	cout;
	}

	// �����������
	// ������ǰ����ָ��
	// ������ "error" "log" "console"
	ostream* SetStream(const char* szStreamName, ostream* pStream);

	// ���������Զ�����
	// iAutoConnect����0��ʾ�Զ�����
	void EnableAutoConnect(int iAutoConnect);

	// ���SQL����еĴ���
	// ����false��ʾ������д���
	bool CheckSQLString(const char* str);

protected:

	long m_lRef;

protected:
	CMutex					m_MutexQuery;
	CMutex					m_MutexConnect;

	int	m_iAutoConnect;	// ����0��ʾ�Զ�����

	string m_host;		// ���ݿ��ַ
	unsigned short m_port;		// ���ݿ�˿�
	string m_database;	// ���ݿ���
	string m_username;	// �ʻ���
	string m_password;	// �ʻ�����

	std::string m_szName;		// ��������

	long			m_errorCode;	// ������			// ���������ӹ����еĴ���
	string			m_errorMessage;	// ������Ϣ�ַ���	// ���������ӹ����еĴ���

public:
	ostream* m_pStreamError;
	ostream* m_pStreamLog;
	ostream* m_pStreamConsole;
};



class KCSQLResultBase : public KCSQLResult
{
public:

	KCSQLResultBase(UINT64 iInsertID)
	{
		m_bStoreResult = true;

		m_numRows = 0;
		m_numFields = 0;
		m_pConnection = NULL;

		m_errorCode = 0;

		m_InsertID = iInsertID;
		if (iInsertID>0)
		{
			m_affectRowCount = 1;
		}
		else
		{
			m_affectRowCount = 0;
		}
	}

	KCSQLResultBase(long affectRowCount)
	{
		m_bStoreResult = true;

		m_numRows = 0;
		m_numFields = 0;
		m_pConnection = NULL;

		m_errorCode = 0;
		m_affectRowCount = affectRowCount;

		m_InsertID = 0;
	}

	KCSQLResultBase(KCSQLConnectionBase* pConnection)
	{
		m_bStoreResult = true;

		m_numRows = 0;
		m_numFields = 0;
		m_pConnection = pConnection;
		if (m_pConnection)
		{
			m_pConnection->AddRef();
		}
		m_affectRowCount = 0;
		m_errorCode = 0;

		m_InsertID = 0;
	}

	// �����ѯ���캯��
	KCSQLResultBase(long errorCode, const char* errorMessage)
	{
		m_bStoreResult = true;

		m_numRows = 0;
		m_numFields = 0;
		m_pConnection = NULL;

		m_affectRowCount = 0;

		m_errorCode = errorCode;
		m_errorMessage = errorMessage;

		m_InsertID = 0;
	}

	virtual ~KCSQLResultBase()
	{
		if (m_pConnection)
		{
			m_pConnection->Release();
			m_pConnection = NULL;
		}
	}

	void Init(KCSQLConnectionBase* pConnection)
	{
		if (m_pConnection)
		{
			m_pConnection->Release();
			m_pConnection = NULL;
		}
		m_pConnection = pConnection;
		if (m_pConnection)
		{
			m_pConnection->AddRef();
		}
	}

	long GetNumRows() const { return m_numRows; }
	long GetNumFields() const { return m_numFields; }

	virtual bool IsNullField(long iFieldOffset) = 0;
	virtual bool IsNullField(const char* szFieldName) = 0;

	virtual int Tell() = 0;
	virtual bool SeekToBegin() = 0;
	virtual bool Seek(int iRowOffset) = 0;

	virtual bool Next() = 0;
	virtual bool IsEnd() = 0;

	virtual bool IsFieldExist(const char* szFieldName);

	virtual const char* GetField(int iFieldOffset);
	virtual const char* GetField(const char* szFieldName);


	virtual const char* GetFieldName(int iFieldOffset);

	virtual const char* operator [](int iFieldOffset);
	virtual const char* operator [](const char* szFieldName);

	virtual bool IsSuccessed();
	virtual long GetErrorCode();
	virtual const char* GetErrorMessage();

	// ȡ��SQLӰ������ݿ������
	virtual long GetAffectRowCount();

	// ��ӡ��ǰ��
	virtual void PrintRow();

	virtual UINT64 GetInsertID() { return m_InsertID; }

public:

	bool			m_bStoreResult;	// �Ƿ��Ǳ��ش洢�Ľ�� ����һ��һ���Ĵ����ݿ�ȡ���

protected:

	int __GetIndex(const char* szName);

protected:

	/// number of rows in the result set or rows affected by the query
	long m_numRows;
	/// number of columns in each row in the result set
	long m_numFields;
	/// true for SELECT type query, false for INSERT, DELETE and UPDATE

	std::vector<string>		m_vName;	// �ֶ���
	std::vector<string>		m_vValue;	// �ֶ�ֵ

	long			m_errorCode;	// ������			// �������ѯ�����еĴ���
	string			m_errorMessage;	// ������Ϣ�ַ���	// �������ѯ�����еĴ���

	long			m_affectRowCount;	// ����Ӱ�������

	std::string				m_Null;		// ��ֵ��������Ч����ʱ�ķ���ֵ
	UINT64					m_InsertID;

	KCSQLConnectionBase*	m_pConnection;	// �ý��ʹ�õ�����
};


#endif //__KC_SQL_BASE_H__


