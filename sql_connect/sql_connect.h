/*************************************************************************
	> File Name: sql_connect.h
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Tue 08 Mar 2016 05:42:00 AM EST
 ************************************************************************/
#ifndef __SQL_CONNECT_H__
#define __SQL_CONNECT_H__

#include<iostream>
#include<string>
#include<stdlib.h>
#include"mysql.h"

class sql_connecter{
	public:
		sql_connecter(const std::string &_host,const std::string &_user,const std::string &_passwd,const std::string &_db);
		bool begin_connect();
		bool insert_sql(const std::string &data);
		bool select_sql(std::string field_name[],std::string _out_data[][5],int &_out_row);
		 ~sql_connecter();
		bool close_connect();
		void show_info();
	private:
		MYSQL_RES *res;
		MYSQL *mysql_base;
		std::string host;
		std::string user;
		std::string passwd;
		std::string db;
};

#endif
