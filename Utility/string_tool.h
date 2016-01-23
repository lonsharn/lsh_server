/*************************************************************************
	> File Name: Framework/string_tool.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年08月13日 星期四 20时53分19秒
 ************************************************************************/

#pragma once
#include "string.h"
#include<iostream>
#include<vector>
#include<string>
#include<functional>

using namespace std;

void Split(const string& str, vector<string>& out, const char* splitter)
{
	out.clear();
	size_t pos = 0;
	while(true)
	{
		size_t temp = str.find(splitter, pos);
		if(temp == -1)
		{
			out.push_back(string(str.begin() + pos, str.end()));
			return;
		}
		else
		{
			if(temp == pos)
			{
				out.push_back(string());
			}
			else
			{
				out.push_back(string(str.begin() + pos, str.begin() + temp));
				pos = temp + strlen(splitter);
			}
		}
	}
}
