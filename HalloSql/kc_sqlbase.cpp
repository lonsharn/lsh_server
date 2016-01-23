

#include "kc_sqlbase.h"
#include "kc_string.h"


void KCSQLConnectionBase::EnableAutoConnect(int iAutoConnect)
{
	m_iAutoConnect = iAutoConnect;
}

bool KCSQLConnectionBase::CheckSQLString(const char* str)
{
	/*
	if (str)
	{
		// 检查是否是合法的SQL语句
		// 没有 where 语句不合法
		while (*str==' ' && *str!='\0')
		{
			++ str;
		}
		if ( strnicmp(str, "select", 6) == 0
			|| strnicmp(str, "update", 6) == 0
			|| strnicmp(str, "delete", 6) == 0 )
		{
			// 必须有 where 语句
			if (strstri(str, "where")==NULL)
			{
				Error() << "错误的SQL语句(无where): " << str << "\r\n";
				return	false;
			}
		}
	}*/
	return	true;
}

ostream* KCSQLConnectionBase::SetStream(const char* szStreamName, ostream* pStream)
{
	if (stricmp(szStreamName, "error")==0)
	{
		ostream* pOldStream = m_pStreamError;
		m_pStreamError = pStream;
		return	pOldStream;
	}
	else if (stricmp(szStreamName, "log")==0)
	{
		ostream* pOldStream = m_pStreamLog;
		m_pStreamLog = pStream;
		return	pOldStream;
	}
	else if (stricmp(szStreamName, "console")==0)
	{
		ostream* pOldStream = m_pStreamConsole;
		m_pStreamConsole = pStream;
		return	pOldStream;
	}
	return	NULL;
}

const char* KCSQLResultBase::GetFieldName(int iFieldOffset)
{
	if (iFieldOffset>=0 && iFieldOffset < (int)m_vValue.size())
	{
		return	m_vName[iFieldOffset].c_str();
	}
	return	m_Null.c_str();
}

bool KCSQLResultBase::IsFieldExist(const char* szFieldName)
{
	return __GetIndex(szFieldName)>=0;
}

long KCSQLResultBase::GetAffectRowCount()
{
	return	m_affectRowCount;
}

const char* KCSQLResultBase::GetField(int iFieldOffset)
{
	if (iFieldOffset>=0 && iFieldOffset < (int)m_vValue.size())
	{
		return	m_vValue[iFieldOffset].c_str();
	}
	return	m_Null.c_str();
}

const char* KCSQLResultBase::GetField(const char* szFieldName)
{
	int iFieldOffset = __GetIndex(szFieldName);

	if (iFieldOffset>=0 && iFieldOffset < (int)m_vValue.size())
	{
		return	m_vValue[iFieldOffset].c_str();
	}

	{
#ifdef _DEBUG
		string	szError;
		char szBuffer[256];
		szBuffer[0] = '\0';
		sprintf(szBuffer, "缺少字段 : %s \r\n现有字段\r\n", szFieldName);
		szError = szBuffer;

		for (int i = 0; i<(int)m_vName.size(); ++i)
		{
			szError += string("\r\n") + m_vName[i];
		}
		if (m_pConnection)
		{
			m_pConnection->Error() << szError;
		}
#endif
	}
	return	m_Null.c_str();
}

const char* KCSQLResultBase::operator [](const char* szFieldName)
{
	return	GetField(szFieldName);
}

const char* KCSQLResultBase::operator [](int iFieldOffset)
{
	return	GetField(iFieldOffset);
}

bool KCSQLResultBase::IsSuccessed()
{
	return (m_errorCode==0);
}

long KCSQLResultBase::GetErrorCode()
{
	return	m_errorCode;
}

const char* KCSQLResultBase::GetErrorMessage()
{
	return	m_errorMessage.c_str();
}

// 打印当前行
void KCSQLResultBase::PrintRow()
{
	string	szText;
	char szBuffer[256];
	szBuffer[0] = '\0';
	sprintf(szBuffer, "该结果共有%u行,%u列\r\n", m_numRows, m_numFields);
	szText = szBuffer;

	for (int i = 0; i<(int)m_vName.size(); ++i)
	{
		szText += string("\r\n") + m_vName[i] + " : " + m_vValue[i];
	}
	szText += "\r\n";
	if (m_pConnection)
	{
		m_pConnection->Console() << szText ;
	}
}

// 取得Field的偏移量
int KCSQLResultBase::__GetIndex(const char* szName)
{
	vector<string>::iterator iter = m_vName.begin();
	for (int i = 0, iCnt = (int)m_vName.size(); i<iCnt; ++i,++iter)
	{
		if ((*iter)==szName)
		{
			return	i;
		}
	}
	return	-1;
}


