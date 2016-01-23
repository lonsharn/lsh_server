
#include "kc_mysql.h"

#ifdef KC_MY_SQL

#include <string>

using namespace std;


#ifdef _WIN32
#include <malloc.h>
#include <crtdbg.h>
#else
#include <alloca.h>
#endif


SQL_API int SQLEscapeString(char *to, const char *from, unsigned int length)
{
	return	mysql_escape_string(to, from, length);
}

SQL_API int SQLEscapeString(string& szSql)
{
	char* szBuffer = (char*)alloca(szSql.length() * 2 + 2);
	szBuffer[0] = '\0';
	int ret = mysql_escape_string(szBuffer, szSql.c_str(), (unsigned long)szSql.length());
	szSql = szBuffer;
	return	ret;
}

SQL_API int SQLEscapeString(string& to, const string& szSql)
{
	char* szBuffer = (char*)alloca(szSql.length() * 2 + 2);
	szBuffer[0] = '\0';
	int ret = mysql_escape_string(szBuffer, szSql.c_str(), (unsigned long)szSql.length());
	to = szBuffer;
	return	ret;
}

bool MySQLConnection::IsConnected()
{
	if (m_conn)
	{
		return	true;
//		return mysql_ping(m_conn)==0;
	}
	return	false;
}

void MySQLConnection::Disconnect()
{
	CMutexGuard		guard(m_MutexConnect);

	if (m_conn)
	{
		mysql_close(m_conn);
		m_conn = NULL;
	}
}

bool IsInsertSQL(const char* szSQL)
{
	while (*szSQL==' ' && *szSQL!='\0')
	{
		++ szSQL;
	}
	if (strnicmp(szSQL, "insert", 6) == 0)
	{
		return	true;
	}
	return	false;
}


KCSQLResult* MySQLConnection::ExecuteQuery(
									const char* queryStr,
									long timeout = -1
									)
{
	return	ExecuteQueryEx(queryStr, timeout, 0);
}


KCSQLResult* MySQLConnection::ExecuteQueryEx(
								  const char* queryStr,
								  long iTimeout,
								  int type
								  )
{
	CMutexGuard		guard(m_MutexQuery);

	if (m_conn == NULL)
	{
		if (m_iAutoConnect)
		{
			if (!Connect(iTimeout))
			{
				return	new MySQLResult(-1, "连接出错");
			}
		}
		else
		{
			return	new MySQLResult(-1, "无数据库连接");
		}
	}

	bool bStoreResult;
	if (type==1)
	{
		// 一条一条取结果
		bStoreResult = false;
	}
	else
	{
		// 取全部结果
		bStoreResult = true;
	}

	if (iTimeout > 0)
	{
		mysql_options(m_conn, MYSQL_OPT_READ_TIMEOUT, (const char*)&iTimeout);
	}

	MySQLResult* pResult = NULL;
	while (1)
	{
		int result = mysql_real_query(m_conn, queryStr, (int)strlen(queryStr));
		if (result!=0)
		{
			// 不成功重试
			if (mysql_ping(m_conn)==0)
			{
				result = mysql_real_query(m_conn, queryStr, (int)strlen(queryStr));
			}
			else
			{
				Connect(iTimeout);
				result = mysql_real_query(m_conn, queryStr, (int)strlen(queryStr));
			}
		}

		if (result!=0)
		{
			break;
		}

		MYSQL_RES* queryResult = NULL;

		if (bStoreResult)
		{
			// 取全部结果
			queryResult = mysql_store_result(m_conn);
		}
		else
		{
			// 一条一条取结果
			queryResult = mysql_use_result(m_conn);
		}

		if (queryResult == NULL)
		{
			if (mysql_errno(m_conn)==0)
			{
				if (IsInsertSQL(queryStr))
				{
					// 没结果但无错误，录影插入ID
					UINT64 iInsertID =  mysql_insert_id(m_conn);
					if (iInsertID>0)
					{
						pResult = new MySQLResult( iInsertID );
					}
					else
					{
						pResult = new MySQLResult( (long)mysql_affected_rows(m_conn) );
					}
				}
				else
				{
					// 没结果但无错误，仅记录影响行数
					pResult = new MySQLResult( (long)mysql_affected_rows(m_conn) );
				}
			}
			break;
		}

		MYSQL_FIELD* pField = mysql_fetch_fields(queryResult);
		if (pField == NULL)
		{
			mysql_free_result(queryResult);
			break;
		}
		else
		{
			pResult = new MySQLResult(this, queryResult, pField, bStoreResult);
		}
		break;
	}

	if (pResult == NULL)
	{
		// 有错误产生
		pResult = new MySQLResult(mysql_errno(m_conn), mysql_error(m_conn));
	}

	pResult->m_bStoreResult = bStoreResult;

	return	pResult;
}

MySQLConnection::MySQLConnection()
{
	m_conn = NULL;
}

MySQLConnection::~MySQLConnection()
{
	Disconnect();
}

bool MySQLConnection::Init(
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

bool MySQLConnection::Connect(int iTimeout)
{
	CMutexGuard		guard(m_MutexConnect);

	m_errorCode = 0;
	m_errorMessage = "";

	if (m_conn)
	{
		mysql_close(m_conn);
		m_conn = NULL;
	}

	m_conn = mysql_init(NULL);
	if (m_conn == NULL)
	{
		Error() << "创建MySQL连接句柄出错\r\n";
		return	false;
	}

	if (iTimeout>0)
	{
		mysql_options(m_conn, MYSQL_OPT_CONNECT_TIMEOUT, (const char*)&iTimeout);
	}

	if ( mysql_real_connect(m_conn, m_host.c_str(), m_username.c_str(), 
		m_password.length()==0 ? (const char*)NULL : m_password.c_str(),
		m_database.c_str(), m_port, NULL, 0))
	{
		Log() << m_szName << "\tMySQL connection to " << m_username << '@' << m_host 
			<< '[' << m_database << "] established successfully\r\n";

		m_errorCode = 0;
		return true;
	}
	else
	{
		Log() << m_szName << "\tMySQL connection to " << m_username << '@' << m_host 
			<< '[' << m_database << "] failed (mysql_real_connect failed): " << mysql_error(m_conn) << "\r\n";

		m_errorCode = mysql_errno(m_conn);
		m_errorMessage = mysql_error(m_conn);

		mysql_close(m_conn);
		m_conn = NULL;
		return	false;
	}
}

int MySQLResult::Tell()
{
	return	m_iCurrentRow;
/*
	MYSQL_ROW_OFFSET offset = mysql_row_tell(m_sqlResult);
	int iRowOffset = offset->
//	int iRowOffset = (int)mysql_row_tell(m_sqlResult);
	if (m_iCurrentRow != iRowOffset)
	{
		if (m_pConnection)
		{
			m_pConnection->Error() << "数据库当前行不匹配\r\n";
		}
		m_iCurrentRow = iRowOffset;
	}
	return	iRowOffset;
*/
}

bool MySQLResult::SeekToBegin()
{
	return Seek(0);
}

bool MySQLResult::Next()
{
	if (m_bStoreResult)
	{
		if (m_iCurrentRow>=0 && m_iCurrentRow + 1<m_numRows)
		{
			MYSQL_ROW row = mysql_fetch_row(m_sqlResult);
			unsigned long* rowLen = mysql_fetch_lengths(m_sqlResult);

			if (row == NULL || rowLen == NULL)
			{
				// 数据结束
				m_sqlRow = NULL;
				m_sqlRowLengths = 0;
				m_iCurrentRow = m_numRows;

				for (int i = 0; i<(int)m_vValue.size(); ++i)
				{
					m_vValue[i].clear();
				}

//				mysql_data_seek(m_sqlResult, m_iCurrentRow);
				return false;
			}
			else
			{
				m_sqlRow = row;
				m_sqlRowLengths = rowLen;

				++ m_iCurrentRow;

				for (int i = 0; i<m_numFields; ++i)
				{
					m_vValue[i].assign(m_sqlRow[i], m_sqlRowLengths[i]);
				}
				return	true;
			}
//			return	Seek( m_iCurrentRow + 1 );
		}
		else
		{
			// 数据结束
			m_sqlRow = NULL;
			m_sqlRowLengths = 0;
			m_iCurrentRow = m_numRows;

			for (int i = 0; i<(int)m_vValue.size(); ++i)
			{
				m_vValue[i].clear();
			}
			return	false;
		}
	}
	else
	{
		// 一条一条的从数据库取结果

		MYSQL_ROW row = mysql_fetch_row(m_sqlResult);
		unsigned long* rowLen = mysql_fetch_lengths(m_sqlResult);

		if (row && rowLen)
		{
			m_sqlRow = row;
			m_sqlRowLengths = rowLen;

			if (m_iCurrentRow == 0)
			{
				// 第一行不加行数
				// 设置列名称
				if (m_pField)
				{
					m_numFields = mysql_num_fields(m_sqlResult);

					for (int i = 0; i<m_numFields; ++i)
					{
						m_vName.push_back(m_pField[i].name);
					}
					m_vValue.resize(m_numFields);
				}
			}
			else
			{
				++ m_iCurrentRow;
			}

			m_numRows = m_iCurrentRow + 1; // 当前行数就是最大行数

			for (int i = 0; i<m_numFields; ++i)
			{
				m_vValue[i].assign(m_sqlRow[i], m_sqlRowLengths[i]);
			}
			return	true;
		}
	}
	return	false;
}

bool MySQLResult::IsEnd()
{
	if (m_bStoreResult)
	{
		return	m_numRows == 0 || m_iCurrentRow == m_numRows;
	}
	else
	{
		return	m_numRows == 0 || (m_sqlResult && mysql_eof(m_sqlResult)!=0);
	}
}

bool MySQLResult::Seek(int iRowOffset)
{
	if (m_bStoreResult)
	{
		// 存储在本地的数据
		if (iRowOffset>=0 && iRowOffset<m_numRows)
		{
			if (iRowOffset!=m_iCurrentRow)
			{
				mysql_data_seek( m_sqlResult, iRowOffset);
				MYSQL_ROW row = mysql_fetch_row(m_sqlResult);
				unsigned long* rowLen = mysql_fetch_lengths(m_sqlResult);

				if (row == NULL || rowLen == NULL)
				{
					mysql_data_seek(m_sqlResult, m_iCurrentRow);
					return false;
				}
				else
				{
					m_sqlRow = row;
					m_sqlRowLengths = rowLen;

					m_iCurrentRow = iRowOffset;

					for (int i = 0; i<m_numFields; ++i)
					{
						m_vValue[i].assign(m_sqlRow[i], m_sqlRowLengths[i]);
					}
					return	true;
				}
			}
		}
	}
	else
	{
		// 一条一条的从数据库取结果
		// 不支持Seek
		return	false;
	}

	return	false;
}

bool MySQLResult::IsNullField(long iFieldOffset)
{
	return m_sqlRow == NULL || iFieldOffset < 0 || iFieldOffset >= m_numFields
		|| m_sqlRow[iFieldOffset] == NULL;
}

bool MySQLResult::IsNullField(const char* szFieldName)
{
	long iFieldOffset = __GetIndex(szFieldName);
	if ( iFieldOffset >= 0 )
	{
		return m_sqlRow == NULL || iFieldOffset < 0 || iFieldOffset >= m_numFields
			|| m_sqlRow[iFieldOffset] == NULL;
	}
	return	false;
}

MySQLResult::MySQLResult(UINT64 iInsertID)
: KCSQLResultBase(iInsertID)
{
	m_numFields = 0;
	m_numRows = 0;
	m_iCurrentRow = -1;
	m_sqlRowLengths = NULL;
	m_sqlResult = NULL;
	m_sqlRow = NULL;

	m_pField = NULL;
}

MySQLResult::MySQLResult(long affectRowCount)
: KCSQLResultBase(affectRowCount)
{
	m_numFields = 0;
	m_numRows = 0;
	m_iCurrentRow = -1;
	m_sqlRowLengths = NULL;
	m_sqlResult = NULL;
	m_sqlRow = NULL;

	m_pField = NULL;
}

MySQLResult::MySQLResult(long errorCode, const char* errorMessage)
: KCSQLResultBase(errorCode, errorMessage)
{
	m_numFields = 0;
	m_numRows = 0;
	m_iCurrentRow = -1;
	m_sqlRowLengths = NULL;
	m_sqlResult = NULL;
	m_sqlRow = NULL;

	m_pField = NULL;
}

MySQLResult::MySQLResult(MySQLConnection* pConnection, MYSQL_RES* sqlResult, MYSQL_FIELD* pField, bool bStoreResult)
:  KCSQLResultBase (pConnection)
{
	assert(pConnection && sqlResult && pField);

	m_numFields = 0;
	m_numRows = 0;
	m_iCurrentRow = -1;
	m_sqlRowLengths = NULL;
	m_sqlResult = NULL;
	m_sqlRow = NULL;

	m_pField = pField;
	m_bStoreResult = bStoreResult;

	if (sqlResult && pField)
	{
		m_sqlResult = sqlResult;

		if (m_bStoreResult)
		{
			m_numRows = (long)mysql_num_rows(m_sqlResult);
			m_numFields = mysql_num_fields(m_sqlResult);

			for (int i = 0; i<m_numFields; ++i)
			{
				m_vName.push_back(pField[i].name);
			}
			m_vValue.resize(m_numFields);

			if (m_numRows>0)
			{
				Seek(0);
			}
			else
			{
				m_iCurrentRow = -1;
			}
		}
		else
		{
			m_iCurrentRow = 0;
			m_numRows = 0;
			Next();
		}
	}
	m_affectRowCount = (long)mysql_affected_rows(pConnection->m_conn);
}

MySQLResult::~MySQLResult()
{
	if (m_sqlResult)
	{
		mysql_free_result(m_sqlResult);
		m_sqlResult = NULL;
	}
	m_sqlRowLengths = NULL;
	m_iCurrentRow = 0;
}

#endif // KC_MY_SQL

