/*************************************************************************
	> File Name: LoginService/main.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月14日 星期五 18时38分04秒
 ************************************************************************/

#include "login_service.h"

#include<unistd.h>
#include<string.h>
#include<iostream>
#include<map>

#include <log4cpp/PropertyConfigurator.hh>

#include "Utility/log4cpp_wrapper.h"
#include "HalloSql/_kcsql.h"
#include "HalloMQ/hallo_mq.h"
#include "HalloMQ/hmq_connection.h"
#include "HalloMQ/hmq_session.h"
#include "HalloMQ/hmq_message.h"

using namespace std;

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

	KCSQLConnection conn;
	if(!conn.Create("pgsql", "192.168.1.151", "hallo", "hallo", "251723", 5432))
	{
		cout<<"connect failed!"<<endl;
		return -1;
	}

	KCSQLResult* result = NULL;
	
	result = conn.ExecuteQuery("create table userinfo(	\
			uid varchar(20) primary key,	\
			reg_type int,	\
			nickname varchar(20))");
	if(!result->IsSuccessed())
	{
		cout<<result->GetErrorMessage()<<endl;
		return -1;
	}
	
	result = conn.ExecuteQuery("insert into userinfo(uid, reg_type, nickname) values('11111', 1, 'hlx')");
	if(!result->IsSuccessed())
	{
		cout<<result->GetErrorMessage()<<endl;
		return -1;
	}

	return 0;

	/*
	while(true)
	{
		INFO(LoginService)<<"this is a test log message!";
	}
	*/
	sleep(-1);
	return 0;
}
