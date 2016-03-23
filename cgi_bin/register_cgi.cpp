/*************************************************************************
	> File Name: register_cgi.cpp
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Sat 12 Mar 2016 11:37:51 PM EST
 ************************************************************************/

#include <iostream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "sql_connect.h" //sql_connect.h

const std::string _remote_ip = "127.0.0.1";
const std::string _remote_user = "helen";
const std::string _remote_passwd = "zyh666";
const std::string _remote_db   = "student";

void fun(char *data)
{
	char *ptr = data;
	while(*ptr != '\0'){
		if(*ptr == '='|| *ptr == '&'){
			*ptr = ' ';
		}
		ptr++;
	}

	char name[64];
	char age[64];
	char school[64];
	char hobby[64];
	memset(name,'\0', sizeof(name));
	memset(age,'\0', sizeof(age));
	memset(school,'\0', sizeof(school));
	memset(hobby,'\0', sizeof(hobby));

	printf("%s %s %d %s\n",name,age,school,hobby);
	sscanf(data,"%*s %s %*s %s %*s %s %*s %s",name,age,school,hobby);

	std::string insert_data;
	insert_data += "'";
	insert_data += name;
	insert_data += "'";
	insert_data += ",";
	insert_data += age;
	insert_data += ",";
	insert_data += "'";
	insert_data += school;
	insert_data += "'";
	insert_data += ",";
	insert_data += "'";
	insert_data += hobby;
	insert_data += "'";
	
	const std::string _host = _remote_ip;
	const std::string _user = _remote_user;
	const std::string _passwd = _remote_passwd;
	const std::string _db   = _remote_db;
	sql_connecter conn(_host, _user, _passwd, _db);
	conn.begin_connect();
	conn.insert_sql(insert_data);
}

int main()
{
	int content_length = -1;
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	memset(method, '\0', sizeof(method));
	memset(query_string, '\0', sizeof(query_string));
	memset(post_data, '\0', sizeof(post_data));

	strcpy(method,getenv("REQUEST_METHOD"));
	if( strcasecmp(method,"GET") == 0 ){
		strcpy(query_string,getenv("QUERY_STRING"));//name=li&age=10
		fun(query_string);	
	}
	else if( strcasecmp(method,"POST") == 0){
		content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0;
		for( ;i<content_length;++i ){
			read(0,&post_data[i],1);
		}
		post_data[i] = '\0';
		fun(post_data);
	}

//	std::cout<<"<html>"<<std::endl;
//	std::cout<<"<head>show student</head>"<<std::endl;
//	std::cout<<"<body>"<<std::endl;


}
