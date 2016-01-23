/*************************************************************************
	> File Name: DataModel/json_serializer.cpp
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年09月25日 星期五 10时11分50秒
 ************************************************************************/
#include "json_serializer.h"

#include<iostream>
using namespace std;

bool JsonSerializer::serialize(string& out, list<pair<string, any> >& lst)
{
	Json::Value root;
	Json::FastWriter writer;
	
	for(auto it:lst)
	{
		recurseRead(root[it.first], it.second);
	}

	out = writer.write(root);
	return true;
}

void JsonSerializer::recurseRead(Json::Value& jv, any& av)
{
	if(av.type() == typeid(int))
	{
		jv = any_cast<int>(av);
	}
	else if(av.type() == typeid(unsigned int))
	{
		jv = any_cast<unsigned int>(av);
	}
	else if(av.type() == typeid(long long int))
	{
		jv = any_cast<long long int>(av);
	}
	else if(av.type() == typeid(unsigned long long int))
	{
		jv = any_cast<unsigned long long int>(av);
	}
	else if(av.type() == typeid(float))
	{
		jv = any_cast<float>(av);
	}
	else if(av.type() == typeid(double))
	{
		jv = any_cast<double>(av);
	}
	else if(av.type() == typeid(bool))
	{
		jv = any_cast<bool>(av);
	}
	else if(av.type() == typeid(string))
	{
		string str = any_cast<string>(av);
		jv = str;
	}
	else if(av.type() == typeid(list<any>))
	{
		auto lst = any_cast<list<any>&>(av);
		int i = 0;
		for(auto it:lst)
		{
			recurseRead(jv[i], it);
			++i;
		}
	}
	else if(av.type() == typeid(list<pair<string, any> >))
	{
		list<pair<string, any> >& lst = any_cast<list<pair<string, any> >&>(av);
		Json::Value tmp;
		for(auto it:lst)
		{
			recurseRead(tmp[it.first], it.second);
		}
		jv.append(tmp);
	}
	else
	{
		cout<<"error wrong type!"<<endl;
	}
}

void JsonSerializer::recurseWrite(Json::Value& jv, any& av)
{
	switch(jv.type())
	{
		case Json::nullValue:
			{
				av = any();
			}
			break;
		case Json::intValue:
			{
				av = jv.asInt();
			}
			break;
		case Json::uintValue:
			{
				av = jv.asUInt();
			}
			break;
		case Json::realValue:
			{
				av = jv.asDouble();
			}
			break;
		case Json::stringValue:
			{
				av = jv.asString();
			}
			break;
		case Json::booleanValue:
			{
				av = jv.asBool();
			}
			break;
		case Json::arrayValue:
			{
				av = any(list<any>());
				auto al = any_cast<list<any>&>(av);
				for(auto i = 0; i<jv.size(); ++i)
				{
					any tmp;
					recurseWrite(jv[i], tmp);
					al.push_back(tmp);
				}
			}
			break;
		case Json::objectValue:
			{
				av = any(list<pair<string, any> >());
				auto al = any_cast<list<pair<string, any> >&>(av);

				auto member = jv.getMemberNames();
				for (auto it = member.begin(); it != member.end(); it++)
				{
					any any;
					Json::Value& value = jv[(*it)];
					recurseWrite(value, any);
					al.push_back(make_pair(*it, any));
				}
			}
			break;
		default:
			break;
	}
}

bool JsonSerializer::deserialize(const string& in, list<pair<string, any> >& lst)
{
	Json::Value root;
	Json::Reader reader;

	reader.parse(in, root);

	Json::Value::Members member;
	member = root.getMemberNames();
	for (auto it = member.begin(); it != member.end(); it++)
	{
		any any;
		auto value = root[(*it)];
		recurseWrite(value, any);
		lst.push_back(make_pair(*it, any));
	}

	return true;
}
