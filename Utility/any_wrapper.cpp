/*************************************************************************
	> File Name: Framework/wrapper_any.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年11月06日 星期五 10时05分55秒
 ************************************************************************/
#include<iostream>
#include "any.h"

std::string any_cast_string(any& a)
{
	if(a.empty())
	{
		return std::string();
	}

	char buffer[100] = {0};
	if(a.type() == typeid(int))
	{
		int value= any_cast<int>(a);
		sprintf(buffer, "%d", value);
	}
	else if(a.type() == typeid(float))
	{
		double value= any_cast<float>(a);
		sprintf(buffer, "%f", value);
	}
	else if(a.type() == typeid(double))
	{
		double value= any_cast<double>(a);
		sprintf(buffer, "%lf", value);
	}
	else if(a.type() == typeid(long))
	{
		long value= any_cast<long>(a);
		sprintf(buffer, "%ld", value);
	}
	else if(a.type() == typeid(long long))
	{
		long long value= any_cast<long long>(a);
		sprintf(buffer, "%lld", value);
	}
	else if(a.type() == typeid(std::string))
	{
		std::string value= any_cast<std::string>(a);
		return value;
	}
	else if(a.type() == typeid(bool))
	{
		bool value= any_cast<bool>(a);
		if(value)
		{
			return std::string("true");
		}
		else
		{
			return std::string("false");
		}
	}
	else
	{
		std::cerr<<"unsupport type"<<std::endl;
	}

	return std::string(buffer);
}
