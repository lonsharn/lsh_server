/*************************************************************************
	> File Name: Utility/log4cpp_wapper.h
	> Author: lonsharn
	> Mail: 275089785@qq.com 
	> Created Time: 2015年10月27日 星期二 16时59分02秒
 ************************************************************************/
#pragma once

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

#define ERROR(type) log4cpp::Category::getInstance(#type).errorStream()<<__FILE__":"<<__LINE__<<"  "
#define INFO(type) log4cpp::Category::getInstance(#type).infoStream()<<__FILE__":"<<__LINE__<<"  "
