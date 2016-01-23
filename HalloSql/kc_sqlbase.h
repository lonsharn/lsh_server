
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
			assert("KCSQLConnectionBase被释放，但引用计数不为0"==0);
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

	// 设置输出流名
	// 返回以前的流指针
	// 可用名 "error" "log" "console"
	ostream* SetStream(const char* szStreamName, ostream* pStream);

	// 激活或禁用自动重连
	// iAutoConnect大于0表示自动重连
	void EnableAutoConnect(int iAutoConnect);

	// 检查SQL语句中的错误
	// 返回false表示该语句有错误
	bool CheckSQLString(const char* str);

protected:

	long m_lRef;

protected:
	CMutex					m_MutexQuery;
	CMutex					m_MutexConnect;

	int	m_iAutoConnect;	// 大于0表示自动重连

	string m_host;		// 数据库地址
	unsigned short m_port;		// 数据库端口
	string m_database;	// 数据库名
	string m_username;	// 帐户名
	string m_password;	// 帐户密码

	std::string m_szName;		// 连接名字

	long			m_errorCode;	// 错误码			// 仅保存连接过程中的错误
	string			m_errorMessage;	// 错误消息字符串	// 仅保存连接过程中的错误

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

	// 错误查询构造函数
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

	// 取得SQL影响的数据库表行数
	virtual long GetAffectRowCount();

	// 打印当前行
	virtual void PrintRow();

	virtual UINT64 GetInsertID() { return m_InsertID; }

public:

	bool			m_bStoreResult;	// 是否是本地存储的结果 还是一条一条的从数据库取结果

protected:

	int __GetIndex(const char* szName);

protected:

	/// number of rows in the result set or rows affected by the query
	long m_numRows;
	/// number of columns in each row in the result set
	long m_numFields;
	/// true for SELECT type query, false for INSERT, DELETE and UPDATE

	std::vector<string>		m_vName;	// 字段名
	std::vector<string>		m_vValue;	// 字段值

	long			m_errorCode;	// 错误码			// 仅保存查询过程中的错误
	string			m_errorMessage;	// 错误消息字符串	// 仅保存查询过程中的错误

	long			m_affectRowCount;	// 操作影响的行数

	std::string				m_Null;		// 空值，遇到无效访问时的返回值
	UINT64					m_InsertID;

	KCSQLConnectionBase*	m_pConnection;	// 该结果使用的连接
};


#endif //__KC_SQL_BASE_H__


