/*************************************************************************
  > File Name: LoginService/login_service.cpp
  > Author: lonsharn
  > Mail: 275089785@qq.com 
  > Created Time: 2015年08月19日 星期三 15时25分09秒
 ************************************************************************/

#include "login_service.h"
#include "login_user.h"

#include <unistd.h>
#include<iostream>
#include<functional>
#include <uuid/uuid.h>

#include "log4cpp/Category.hh"

enum LoginType
{
	LoginType_Invalid	= 0,
	LoginType_WeiXin	= 1,
	LoginType_Phone		= 2,
	LoginType_Auto		= 3,
	LoginType_UserName	= 4
};

using namespace std;

bool LoginService::VerifyUser(const string& name, const string& passwd, int plat)
{
	return true;
}

void LoginService::AddUser(LoginUser* user)
{
	m_uid_list.insert(make_pair(user->getValue<string>("uid"), user));
	m_phone_list.insert(make_pair(user->getValue<string>("phone"), user));
}

bool LoginService::RemoveUser(string& uid)
{
	auto it = m_phone_list.find(uid);
	if(it == m_phone_list.end())
	{
		return false;
	}
	else
	{
		delete it->second;
	}

	m_phone_list.erase(it);
	return true;
}
