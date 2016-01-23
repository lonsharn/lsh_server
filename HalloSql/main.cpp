

#include <iostream>

#include "_kcsql.h"

#ifdef _WIN32
#define SQL_HOST_ADDR "192.168.1.99"
#else
#define SQL_HOST_ADDR "127.0.0.1"
#endif


#ifdef _WIN32
#ifdef _USEDLL
BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{
	return TRUE;
}
#endif
#endif


#if 0
int main(int argc, char* argv[])
{
/*
	KCSQLConnection* pConnection = CreateSQLConnection(
						"mysql",
						"192.168.1.99",
						"KC_DB",
						"zjl",
						"123456",
						3306
						);
*/

{
	KCSQLConnection		conn;

	bool bSuccess = conn.Create(
		"mysql",
		SQL_HOST_ADDR,
		"test",
		"zjl",
		"123456",
		3306
		);

	if (bSuccess)
	{
		// 重定向消息输出
		conn.SetStream("error", &cout);
		conn.SetStream("log", &cout);
		conn.SetStream("console", &cout);

		KCSQLResult* pResult = conn.ExecuteQuery("insert into T_Area(Code,National,Province,City,Name) values('1000001','12','01','33','somebody')", -1);
		if (pResult)
		{
			if (pResult->IsSuccessed())
			{
				int effectCnt = pResult->GetAffectRowCount();
			}
			delete pResult;
			pResult = NULL;
		}

		pResult = conn.ExecuteQuery("select * from T_Area", -1);

		if (pResult)
		{
			if (pResult->IsSuccessed())
			{
				int iRowCnt = pResult->GetNumRows();
				if (iRowCnt>0)
				{
					do
					{
						pResult->PrintRow();
					}
					while (pResult->Next());
				}
			}
		}
	}
}
//////////////////////////////////////////////////////
/*
{
	KCSQLConnection		conn;

	bool bSuccess = conn.Create(
		"mysql",
		SQL_HOST_ADDR,
		"KC_DB",
		"zjl",
		"123456",
		3306
		);

	if (bSuccess)
	{
		// 重定向消息输出
		conn.SetStream("error", &cout);
		conn.SetStream("log", &cout);
		conn.SetStream("console", &cout);

		KCSQLResult* pResult = conn.ExecuteQuery("select * from T_Area", -1);
		if (pResult && pResult->IsSuccessed())
		{
			int iRowCnt = pResult->GetNumRows();
			if (iRowCnt>0)
			{
				do
				{
					pResult->PrintRow();
				}
				while (pResult->Next());
			}

			// 访问第11行数据，参数10为索引
			if (pResult->Seek(10))
			{
				// 第3列
				cout << pResult->GetField(2) << "\r\n";
				// 用名字访问
				cout << pResult->GetField("name") << "\r\n";
			}
		}


		if (pResult)
		{
			delete pResult;
		}
	}
}
*/
////////////////////////////////////////////////////////////////////

	{
	KCSQLConnection		conn;

	bool bSuccess = conn.Create(
		"pgsql",
		SQL_HOST_ADDR,
		"voipdb",
		"gkradius",
		"keepc_radius",
		5432
		);

	if (bSuccess)
	{
		// 重定向消息输出
		conn.SetStream("error", &cout);
		conn.SetStream("log", &cout);
		conn.SetStream("console", &cout);

		KCSQLResult* pResult = conn.ExecuteQuery("select * from voipuser", -1);
		if (pResult)
		{
			if (pResult->IsSuccessed())
			{
				int iRowCnt = pResult->GetNumRows();
				if (iRowCnt>0)
				{
					do
					{
						pResult->PrintRow();
					}
					while (pResult->Next());
				}
			}
			delete pResult;
			pResult = NULL;
		}
	}
	}
/*
	if (pConnection)
	{
		pConnection->AllInfoToStream(&cout);
		KCSQLResult* pResult = pConnection->ExecuteQuery("select * from T_Area", -1);
		if (pResult)
		{
			int iRowCnt = pResult->GetNumRows();
			if (iRowCnt>0)
			{
				do
				{
					pResult->PrintRow();
				}
				while (pResult->Next());
			}
		}
	}
*/
	char c;
	cin >> c;
	return 0;
}

#endif

