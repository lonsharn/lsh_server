/*************************************************************************
	> File Name: HwSql/sql_stmt.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月15日 星期二 16时01分24秒
 ************************************************************************/
#pragma once
#include<iostream>
#include<vector>

#include "Utility/any.h"
using namespace std;

class SqlConnect;
enum StmtStepRet
{
	StmtStepRet_Done = 10,
	StmtStepRet_Row,
	StmtStepRet_Error,
	StmtStepRet_Busy,
};

enum StmtColumnType
{
	StmtColumnType_Int = 10,
	StmtColumnType_Double,
	StmtColumnType_Long,
	StmtColumnType_Blob,
	StmtColumnType_Null,
};

enum StmtBindResult
{
	StmtBindResult_OK = 0,	//
	StmtBindResult_TooBig,	//数据太大
	StmtBindResult_Range,	//index不在参数范围内
};

class SqlStmt
{
	public:
		SqlStmt() = default;
		virtual void Prepare(int id, const char* sql);
		virtual int Bind(int idx, any&&);
		virtual int Step();
		virtual void Reset();
		virtual void Clear();
		virtual any& GetColumnValue(int field);
		virtual void SetColumnValue(int field, any&& value);
		virtual int GetColumnCount();
		virtual int GetColumnType(int idx);
		virtual ~SqlStmt();
		int GetID();
	protected:
		virtual int BindInt(int idx, int value);
		virtual int BindDouble(int idx, double value);
		virtual int BindInt64(int idx, long value);
		virtual int BindText(int idx, const char* value);
		virtual int BindBlob(int idx, const void* value, long length);
		virtual int GetBindError(int error);
	protected:
		vector<any> m_values;
		int			m_id;
};
