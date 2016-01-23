/*************************************************************************
	> File Name: LoginService/login_user.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月14日 星期五 18时40分10秒
 ************************************************************************/
#pragma once

#include<iostream>

#include "DataModel/property.h"
using namespace std;

class LoginService;
class LoginUser:public Property<LoginUser>
{
	public:
		LoginUser(unsigned long uid, int plat, LoginService*);
		LoginUser(const LoginUser&) = delete;
		~LoginUser();

	private:
		string m_token;
		int m_plat;
		unsigned long m_uid;
		string m_Name;
		LoginService* m_Service = nullptr;
};
