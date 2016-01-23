/*************************************************************************
	> File Name: LoginService/login_user.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月14日 星期五 18时39分31秒
 ************************************************************************/

#include<iostream>
#include "login_user.h"
#include "login_service.h"

using namespace std;

LoginUser::~LoginUser()
{
}

LoginUser::LoginUser(unsigned long uid, int plat, LoginService* service)
{
	m_Service = service;
	m_uid = uid;
	m_plat = plat;
}
