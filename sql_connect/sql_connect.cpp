/*************************************************************************
	> File Name: sql_connect.cpp
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Tue 08 Mar 2016 05:42:14 AM EST
 ************************************************************************/

#include"sql_connect.h"


sql_connecter::sql_connecter(const std::string &_host,const std::string &_user,const std::string &_passwd,const std::string &_db)
{
	mysql_base = mysql_init(NULL);
	res = NULL;
	host = _host;
	user = _user;
	passwd = _passwd;
	db = _db;
}
bool sql_connecter::begin_connect()
{
	if( mysql_real_connect(mysql_base,host.c_str(),user.c_str(),passwd.c_str(),db.c_str(),3306,NULL,0) == NULL ){
		std::cout<<"error"<<std::endl;
		return false;
	}
	else{
		std::cout<<"success"<<std::endl;
		std::cout<<"host.c_str()"<<host.c_str()<<"\t";
		std::cout<<"db.c_str()"<<db.c_str()<<"\t";
		std::cout<<std::endl;
	}
	return true;
	
}
bool sql_connecter::insert_sql(const std::string &data)
{
	std::string sql = "insert into student_info (name,age,school,hobby) values";
	sql += "(";
	sql += data;
	sql += ");";
	if(mysql_query(mysql_base,sql.c_str()) == 0){
		std::cout<<"query success"<<std::endl;
		return true;
	}
	else{
		std::cerr<<"query failed!"<<std::endl;
		return false;
	}
}
bool sql_connecter::select_sql(std::string field_name[],std::string _out_data[][5],int &_out_row)
{
	std::string sql = "select * from student_info";
	if(mysql_query(mysql_base,sql.c_str()) == 0){
		std::cout<<"query connect success"<<std::endl;
	}
	else{
		std::cout<<"query connect failed"<<std::endl;
	}
	res = mysql_store_result(mysql_base);
	int row_num = mysql_num_rows(res);
	int field_num = mysql_num_fields(res);
	_out_row = row_num;

	std::cout<<"row_num:"<<row_num<<std::endl;
	std::cout<<"field_num"<<field_num<<std::endl;

	MYSQL_FIELD *fd = NULL;
	int i = 0;
	for(;fd = mysql_fetch_field(res);){
//		std::cout<<fd->name<<"\t";
		field_name[i++] = fd->name;
	}
	//std::cout<<std::endl;

	for(int index= 0;index<row_num;index++){
		MYSQL_ROW _row = mysql_fetch_row(res);
		if(_row){
			int start = 0;
			for(;start<field_num;++start){
				_out_data[index][start] = _row[start];
	//			std::cout<<_row[start]<<"\t";
			}
			std::cout<<std::endl;
		}
	}
	return true;
}
sql_connecter::~sql_connecter()
{
	close_connect();
	if(res != NULL){
		free(res);
	}
}
bool sql_connecter::close_connect()
{	
	mysql_close(mysql_base);
}

void sql_connecter::show_info()
{
	std::cout<<mysql_get_client_info()<<std::endl;
}

#ifdef _DEBUG_
int main()
{	
	std::string _sql_data[1024][5];
	std::string header[5];

	int curr_row = -1;
	const std::string _host = "127.0.0.1";
	const std::string _user = "helen";
	const std::string _passwd = "zyh666";
	const std::string _db = "student";
	const std::string data = "'hill',24,'xgd','sleep'";

	sql_connecter conn(_host,_user,_passwd,_db);
	conn.begin_connect();
	conn.select_sql(header,_sql_data,curr_row);
	sleep(1);

	for(int i = 0;i<curr_row;++i){
		for(int j = 0;j<5;++j){
			std::cout<<_sql_data[i][j] <<"\t";
		}
		std::cout<<std::endl;
	}
	conn.insert_sql(data);
//	conn.show_info();
	return 0;
}
#endif
