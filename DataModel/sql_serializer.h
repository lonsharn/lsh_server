/**
 * @file sql_serializer.h
 * @brief class SqlSerializer
 * @author lonsharn
 * @version 1.0
 * @date 2015-11-05
 */
#pragma once

#include<string>
#include<list>

#include "serializer.h"
#include "attribute.h"

class KCSQLConnection;
class SqlSerializer:public Serializer
{
	public:
		/**
		 * @brief constructor
		 *
		 * @param conn database handler
		 * @param db database name
		 * @param table table name
		 */
		SqlSerializer(KCSQLConnection& conn, const list<Attribute*>& attr_lst, bool update = true);

		/**
		 * @brief 
		 */
		~SqlSerializer() = default;

		/**
		 * @brief 表是否存在
		 *
		 * @param table_name
		 *
		 * @return 结果
		 */
		bool tableExist(const std::string& table_name);

		/**
		 * @brief 更新表数据
		 *
		 * @param tableName 表名字
		 * @param lst 数据集
		 *
		 * @return 数据库查询是否成功
		 */
		bool updateValue(std::string& tableName, std::list<std::pair<std::string, any> >& lst);

		/**
		 * @brief 更新表数据
		 *
		 * @param tableName 表名字
		 * @param lst 数据集
		 *
		 * @return 数据库查询是否成功
		 */
		bool insertValue(std::string& tableName, std::list<std::pair<std::string, any> >& lst);

		/**
		 * @brief 序列化接口
		 *
		 * @param data 需要写入的表的表名
		 * @param lst 序列化前的结构
		 *
		 * @return 序列化结构
		 */
		bool serialize(std::string& data, list<pair<std::string, any> >& lst);

		/**
		 * @brief 反序列化接口
		 *
		 * @param data 需要读的表名
		 * @param lst 读取数据库表的
		 *
		 * @return 序列化结果
		 */
		bool deserialize(const std::string& data, std::list<std::pair<std::string, any> >& lst);

		/**
		 * @brief 查询数据
		 *
		 * @param tableName 表名
		 * @param lst 查询结果
		 *
		 * @return 查询是否成功
		 */
		bool selectValue(std::string& tableName, list<pair<std::string, any> >& lst);
	private:
		KCSQLConnection& m_conn;
		const list<Attribute*> m_attr_lst;
        bool    m_bUpdate = true;
};
