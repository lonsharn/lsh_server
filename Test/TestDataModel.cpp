/**
 * @file TestDataModel.cpp
 * @brief DataModel test file
 * @author lonsharn
 * @version 1.0
 * @date 2015-11-04
 */

#include<unistd.h>
#include<string.h>
#include<iostream>
#include<map>

#include <uuid/uuid.h>

#include "property.h"
#include "json_serializer.h"
#include "sql_serializer.h"
#include "Utility/log4cpp_wrapper.h"

#include "HalloSql/_kcsql.h"

using namespace std;

class Test:public Property<Test>
{
};

template <>
void Property<Test>::Init()
{
	addAttr("uid:sql_tn=userinfo,sql_pk,sql_nn,sql_fn=uid");
	addAttr("reg_type:sql_tn=userinfo,sql_nn,sql_fn=reg_type");
	addAttr("name:sql_tn=userinfo,sql_nn,sql_fn=nickname");
}

void testSqlSerializer()
{
	KCSQLConnection conn;
	if(!conn.Create("pgsql", "127.0.0.1", "hallo", "hallo", "251723", 5432))
	{
		cout<<"connect failed!"<<endl;
		return ;
	}

	Test t;
	t.setValue("uid", string("f1318f7c-6bf8-41da-a207-fa2ae40e6ef9"));
	t.setValue("reg_type", 1);
	t.setValue("name", string("lonsharn"));

	string tableName("userinfo");
	SqlSerializer s(conn, t.getAttribute(), false);
	t.Serialize(tableName, &s);
}

int main(int argc, char** argv)
{
	try
	{
		log4cpp::PropertyConfigurator::configure("./log4cpp.conf");
	}
	catch (log4cpp::ConfigureFailure& f)
	{
		std::cout << "Configure Problem " << f.what() << std::endl;
		return -1;
	}

	Test::Init();

	testSqlSerializer();
	/*
	string out;
	{
		Test tst;
		tst.setValue("name", string("hlx"));
		tst.setValue("sex", true);

		JsonSerializer serializer;
		tst.Serialize(out, &serializer);
		cout<<out;
	}
	{
		Test tst;
		JsonSerializer s;
		tst.unSerialize(out, &s);
		string name = tst.getValue<string>("name");
		bool sex = tst.getValue<bool>("sex");
		cout<<std::boolalpha;
		cout<<"name="<<name<<",sex="<<sex<<endl;
		tst.Serialize(out, &s);
		cout<<out;
	}
	*/
	return 0;
}
