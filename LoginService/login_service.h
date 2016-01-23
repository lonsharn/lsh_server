/*************************************************************************
	> File Name: LoginService/login_service.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月19日 星期三 15时25分13秒
 ************************************************************************/
#pragma once

#include<iostream>
#include<unordered_map>

using namespace std;

class LoginUser;
class LoginService
{
	public:
		LoginService() = default;
		LoginService(const LoginService&) = delete;
		~LoginService() = default;
		bool VerifyUser(const string& name, const string& passwd, int plat);
		void AddUser(LoginUser* user);
		bool RemoveUser(string& uid);
	private:
		unordered_map<string , LoginUser*> m_uid_list;
		unordered_map<string , LoginUser*> m_phone_list;
		unordered_map<string , LoginUser*> m_email_list;
		unordered_map<string , LoginUser*> m_username_list;
};
