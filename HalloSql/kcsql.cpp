// KCSQL.cpp : Defines the entry point for the console application.
//

#include "_kcsql.h"

#ifdef KC_MY_SQL
#include "kc_mysql.h"
#endif // KC_MY_SQL

#ifdef KC_PG_SQL
#include "kc_pgsql.h"
#endif // KC_PG_SQL

#include "kc_sqlbase.h"

#include <sstream>

using namespace std;

/*
KCSQLConnectionBase* CreateSQLConnection(
							const char* szSQLType,	// 数据库种类
							const char* host,
							const char* database,
							const char* username,
							const char* password,
							unsigned short port
							)
{
	KCSQLConnectionBase* pConnection = NULL;
	if (szSQLType)
	{
		if (strcmpi(szSQLType, "mysql")==0)
		{
			pConnection	= new MySQLConnection();
			pConnection->Init(host, database, username, password, port);
		}
		else if (strcmpi(szSQLType, "pgsql")==0)
		{
		}
	}
	return	pConnection;
}
*/

KCSQLConnection::KCSQLConnection()
{
	m_This = NULL;
}

KCSQLConnection::~KCSQLConnection()
{
	if (m_This)
	{
		m_This->Release();
		m_This = NULL;
	}
}

bool KCSQLConnection::Create(
		  const char* szSQLType,	// 数据库种类
		  const char* host,
		  const char* database,
		  const char* username,
		  const char* password,
		  unsigned short port
		  )
{
	if (m_This)
	{
		m_This->Release();
		m_This = NULL;
	}

	KCSQLConnectionBase* pConnection = NULL;
	if (szSQLType)
	{
		if (stricmp(szSQLType, "mysql")==0)
		{
#ifdef KC_MY_SQL
			pConnection	= new MySQLConnection();
			pConnection->Init(host, database, username, password, port);
#else
			Log() << "this kcsql lib version is not correct, not support mysql" << endl;
#endif // KC_MY_SQL
		}
		else if (stricmp(szSQLType, "pgsql")==0)
		{
#ifdef KC_PG_SQL
			pConnection	= new PgSQLConnection();
			pConnection->Init(host, database, username, password, port);
#else
			Log() << "this kcsql lib version is not correct, not support pgsql" << endl;
#endif // KC_PG_SQL
		}
	}

	if (pConnection)
	{
		m_This = pConnection;
		return	m_This->Connect();
	}
	else
	{
		return	false;
	}
}

KCSQLResult* KCSQLConnection::ExecuteQuery(
	const char* queryStr,
	long iTimeout
	)
{
	return	ExecuteQueryEx(queryStr, iTimeout, EXEC_SQL_STORE_RESULT);
}

KCSQLResult* KCSQLConnection::ExecuteQueryEx(
							const char* queryStr,
							long iTimeout,
							int	type
							)
{
	if (m_This)
	{
		// 检查语法
		if (m_This->CheckSQLString(queryStr))
		{
			KCSQLResult* pResult = m_This->ExecuteQueryEx(queryStr, iTimeout, type);
			if (pResult)
			{
#ifdef _DEBUG
				if (!pResult->IsSuccessed())
				{
					stringstream str;
					str << "执行失败SQL语句 " << queryStr << endl
						<< "error code:" << pResult->GetErrorCode() 
						<< " " << pResult->GetErrorMessage() << endl;
					Error() << str.str();
				}
#endif
			}
			return	pResult;
		}
		else
		{
			string	szError("SQL 语句不符合过滤条件 ");
			szError += queryStr;
#ifdef KC_MY_SQL
			return	new MySQLResult(-101, szError.c_str());
#else
#ifdef KC_PG_SQL
			return	new PgSQLResult(-101, szError.c_str());
#else
# error must define one of KC_MY_SQL or KC_PG_SQL;
#endif
#endif
		}
	}

#ifdef KC_MY_SQL
	return	new MySQLResult(-100,"连接对象未创建 调用 KCSQLConnection::Create()");
#else
#ifdef KC_PG_SQL
	return	new PgSQLResult(-100,"连接对象未创建 调用 KCSQLConnection::Create()");
#else
# error must define one of KC_MY_SQL or KC_PG_SQL;
#endif
#endif
}

bool KCSQLConnection::Connect(int iTimeout)
{
	if (m_This)
	{
		return	m_This->Connect(iTimeout);
	}
	return	false;
}

void KCSQLConnection::Disconnect()
{
	if (m_This)
	{
		m_This->Disconnect();
	}
}

bool KCSQLConnection::IsConnected()
{
	if (m_This)
	{
		return m_This->IsConnected();
	}
	return	false;
}

const char* KCSQLConnection::GetName()
{
	if (m_This)
	{
		m_This->GetName();
	}
	return	NULL;
}

void KCSQLConnection::SetName(const char* szName)
{
	if (m_This)
	{
		m_This->SetName(szName);
	}
}

ostream& KCSQLConnection::Error()
{
	if (m_This)
	{
		return m_This->Error();
	}
	return	cerr;
}

ostream& KCSQLConnection::Log()
{
	if (m_This)
	{
		return m_This->Log();
	}
	return	cerr;
}

ostream& KCSQLConnection::Console()
{
	if (m_This)
	{
		return m_This->Console();
	}
	return	cout;
}

ostream* KCSQLConnection::SetStream(const char* szStreamName, ostream* pStream)
{
	if (m_This)
	{
		return m_This->SetStream(szStreamName, pStream);
	}
	return	NULL;
}

// 激活或禁用自动重连
// iAutoConnect大于0表示自动重连
void KCSQLConnection::EnableAutoConnect(int iAutoConnect)
{
	if (m_This)
	{
		return m_This->EnableAutoConnect(iAutoConnect);
	}
}



