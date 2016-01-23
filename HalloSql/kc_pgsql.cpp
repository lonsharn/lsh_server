

#include "kc_pgsql.h"

#ifdef KC_PG_SQL

PgSQLConnection::PgSQLConnection()
{
	m_conn = NULL;
}

PgSQLConnection::~PgSQLConnection()
{
	if (m_conn)
	{
		PQfinish(m_conn);
	}
}

bool PgSQLConnection::Init(
				  const char* host,
				  const char* database,
				  const char* username,
				  const char* password,
				  unsigned short port
				  )
{
	if (m_conn)
	{
		Error() << "连接已经创建\r\n";
		return	false;
	}

	m_host = host;
	m_database = database;
	m_username = username;
	m_port = port;
	m_password = password;

	return	true;
}


KCSQLResult* PgSQLConnection::ExecuteQuery(
	const char* queryStr,
	long timeout
	)
{
	return	ExecuteQueryEx(queryStr, timeout, 0);
}


KCSQLResult* PgSQLConnection::ExecuteQueryEx(
								  const char* queryStr,
								  long iTimeout,
								  int type
								  )
{
	CMutexGuard		guard(m_MutexQuery);

	if (m_iAutoConnect)
	{
		if (m_conn)
		{
			if ( PQstatus(m_conn) == CONNECTION_BAD )
			{
				PQreset(m_conn);
				// 重试一次不行,就关闭该连接
				if (PQstatus(m_conn) == CONNECTION_BAD)
				{
					PQfinish(m_conn);
					m_conn = NULL;
				}
			}
		}

		if (m_conn == NULL)
		{
			if ( !Connect(iTimeout) )
			{
				return	new PgSQLResult(-1, "连接出错");
			}
		}
	}

	if (m_conn == NULL)
	{
		return	new PgSQLResult(-1, "无数据库连接");
	}

	PGresult* result = PQexec(m_conn, queryStr);
	if (result == NULL)
	{
		// 重试
		Connect(iTimeout);
		result = PQexec(m_conn, queryStr);
	}

	if (result == NULL)
	{
		return	NULL;
	}

	ExecStatusType resultInfo = PQresultStatus(result);
	m_errorCode = resultInfo;

	switch (resultInfo)
	{
	case PGRES_COMMAND_OK:
		// 仅记录影响行数
		return	new PgSQLResult( PQcmdTuples(result) ? atoi(PQcmdTuples(result)) : 0 );
		break;
	case PGRES_TUPLES_OK:
		return	new PgSQLResult(this, result);
		break;

	default:
		return new PgSQLResult(resultInfo, PQresultErrorMessage(result));
		break;
	}
}

bool PgSQLConnection::Connect(int iTimeout)
{
	CMutexGuard		guard(m_MutexConnect);

	if (m_conn)
	{
		PQfinish(m_conn);
		m_conn = NULL;
	}

	char szPort[32];
	szPort[0] = '\0';
	sprintf(szPort, "%i", (int)m_port);

	m_conn = PQsetdbLogin(m_host.c_str(), 
		m_port ? (const char*)szPort : (const char*)NULL,
		NULL /*(const char*)optionsStr*/, NULL,
		m_database.c_str(), m_username.c_str(), 
		m_password.length()==0 ? (const char*)NULL : (const char*)m_password.c_str()
		);

	if (m_conn && PQstatus(m_conn) == CONNECTION_OK)
	{
		Log() << GetName() << "\tPgSQL connection to " << m_username << '@' << m_host 
			<< '[' << m_database << "] established successfully" ;
		return	true;
	}
	else
	{
		Log() << GetName() << "\tPgSQL connection to " << m_username << '@' << m_host 
			<< '[' << m_database << "] failed (PQsetdbLogin failed): " 
			<< (m_conn ? PQerrorMessage(m_conn) : "") ;
		if (m_conn)
		{
			PQfinish(m_conn);
			m_conn = NULL;
		}
		return	false;
	}
}

void PgSQLConnection::Disconnect()
{
	if (m_conn)
	{
		PQfinish(m_conn);
		m_conn = NULL;
	}
}

bool PgSQLConnection::IsConnected()
{
	if (m_conn)
	{
		if ( PQstatus(m_conn) != CONNECTION_BAD )
		{
			return	true;
		}
	}
	return	false;
}

PgSQLResult::PgSQLResult(long affectRowCount)
: KCSQLResultBase(affectRowCount)
{
	m_sqlResult = NULL;
	m_sqlRow = -1;
}

PgSQLResult::PgSQLResult(long errorCode, const char* errorMessage)
: KCSQLResultBase (errorCode, errorMessage)
{
	m_sqlResult = NULL;
	m_sqlRow = -1;
}

PgSQLResult::PgSQLResult(PgSQLConnection* pConnection, PGresult * sqlResult)
: KCSQLResultBase(pConnection)
{
	m_sqlResult = sqlResult;
	if (m_sqlResult)
	{
		m_numRows = PQntuples(m_sqlResult);
		m_numFields = PQnfields(m_sqlResult);
		m_sqlRow = -1;

		for (int i = 0; i<m_numFields; ++i)
		{
			m_vName.push_back( PQfname(m_sqlResult, i) );
		}
		m_vValue.resize(m_numFields);
		const char* pszResult = PQcmdTuples(sqlResult);
		if (pszResult)
		{
			m_affectRowCount = atoi(pszResult);
		}
		Seek(0);
	}
	else
	{
		m_sqlRow = -1;
		m_numRows = 0;
		m_numFields = 0;
	}
}


PgSQLResult::~PgSQLResult()
{
	if (m_sqlResult)
	{
		PQclear(m_sqlResult);
		m_sqlResult = NULL;
	}
}

bool PgSQLResult::SeekToBegin()
{
	return Seek(0);
}

bool PgSQLResult::Next()
{
	if (m_sqlRow>=0 && m_sqlRow + 1<m_numRows)
	{
		return	Seek( m_sqlRow + 1 );
	}
	return	false;
}

bool PgSQLResult::IsEnd()
{
	return	m_sqlRow + 1 == m_numRows;
}

bool PgSQLResult::Seek(int iRowOffset)
{
	if (m_sqlResult == NULL || iRowOffset < 0 || iRowOffset >= m_numRows)
		return false;

	m_sqlRow = iRowOffset;

	for (int i = 0; i<m_numFields; ++i)
	{
		m_vValue[i].assign( PQgetvalue(m_sqlResult, iRowOffset, i),
							PQgetlength(m_sqlResult, iRowOffset, i) );
	}
	return	true;
}

int PgSQLResult::Tell()
{
	return	m_sqlRow;
}

bool PgSQLResult::IsNullField(long iFieldOffset)
{
	return m_sqlResult == NULL || m_sqlRow < 0 || m_sqlRow >= m_numRows
		|| iFieldOffset < 0 || iFieldOffset >= m_numFields
		|| PQgetisnull(m_sqlResult, m_sqlRow, iFieldOffset);
}

bool PgSQLResult::IsNullField(const char* szFieldName)
{
	int iFieldOffset = __GetIndex(szFieldName);

	return m_sqlResult == NULL || m_sqlRow < 0 || m_sqlRow >= m_numRows
		|| iFieldOffset < 0 || iFieldOffset >= m_numFields
		|| PQgetisnull(m_sqlResult, m_sqlRow, iFieldOffset);
}

#endif // KC_PG_SQL
