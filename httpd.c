/*************************************************************************
	> File Name: httpd.c
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Sat 05 Mar 2016 09:05:55 PM EST
 ************************************************************************/

#include"httpd.h"

void clear_header(int sock)
{
	int ret = -1;
	char buf[COMM_SIZE];
	memset(buf,'\0',sizeof(buf));
	do{
		int ret = get_line(sock,buf,sizeof(buf));
	}while(ret > 0 && strcmp(buf,"\n"));
}

void Usage(const char *proc)
{
	printf("%s [PORT]\n",proc);
}
void print_log(const char *func,int line,int errno,const char *err_msg)
{
//	printf("%s %d %d %s\n",func,line,errno,err_msg);
}


//sucess:return the number of the char that read
//failed:return <=0
int get_line(int sock,char *buf,int max_size)
{
	if(NULL == buf || max_size < 0){
		//print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		return -1;
	}

	char c = '\0';
	int i = 0;
	int ret = 0;

	while(i<max_size-1 && c != '\n'){//-------------------------------------
    	ret = recv(sock,&c,1,0);
    	if(ret > 0){//success
            if(c == '\r'){
           	ret = recv(sock,&c,1,MSG_PEEK);
           	if(ret > 0 && c == '\n'){//windows
    	   		recv(sock,&c,1,0);
    	   	 }else{
           		c = '\n';
    	   	}
    	   }
    	
    	buf[i++] = c;
//		printf("%c ",c);
    	}else{//failed
    	   c = '\n';//break
    	}

	}
	buf[i] = '\0';
//	printf("------------------------get_line-------------------->%s\n",buf);
	return i;
}

void echo_error_to_client()
{}

void printf_debug(const char *msg)
{
	printf("%s\n",msg);
}

void execute_cgi(int sock_client,const char* path,const char *method,const char *query_string)
{
	printf_debug("enter execute_cgi");
	printf("method is %s\n",method);
	printf("path is %s\n",path);
	printf("query_string is %s\n",query_string);

	char buf[COMM_SIZE];
	int numchars = 0;
	int content_length = -1;

	//pipe
	int cgi_input[2]  = {0,0};
	int cgi_output[2] = {0,0};

	//child proc
	pid_t id;
	printf_debug(method);

	if(strcasecmp(method,"GET") == 0){//GET
		printf_debug("clear_header");
		clear_header(sock_client);
	}
	else{//POST
		do{
			memset(buf,'\0',sizeof(buf));
			numchars = get_line(sock_client,buf,sizeof(buf));
			if(strncasecmp(buf,"Content-Length:",strlen("Content-Length:")) == 0){
				content_length = atoi(&buf[16]);
			}
		}while(numchars > 0 && strcmp(buf,"\n") != 0);

		if(content_length == -1){
			//echo_error_to_client();
			return;
		}
	}

	memset(buf,'\0',sizeof(buf));
	strcpy(buf,HTTP_VERSION);
	strcat(buf," 200 OK\r\n\r\n");
	printf_debug("begin send echo_line");
	send(sock_client,buf,strlen(buf),0);//------------------------------------------>
	printf_debug("send echo_line success");
	

	if( pipe(cgi_input) == -1){//pipe error
		//echo_error_to_client
		return;
	}

	if( pipe(cgi_output) == -1 ){//pipe error
		close(cgi_input[0]);
		close(cgi_input[1]);
		//echo_error_to_clent()
		return;
	}

	if((id = fork()) < 0){//fork error//??????????????????????????()can not miss
		printf_debug("fork error");
		close(cgi_input[0]);
		close(cgi_input[1]);
		close(cgi_output[0]);
		close(cgi_output[1]);
		//echo error_to_client()
		return;
	}
	else if( id == 0){//child
    	printf_debug("enter child proc");
		char query_env[COMM_SIZE];
		char method_env[COMM_SIZE];
		char content_len_env[COMM_SIZE];
	
		memset(query_env,'\0',sizeof(query_env));
		memset(method_env,'\0',sizeof(method_env));
		memset(content_len_env,'\0',sizeof(content_len_env));
	
		close(cgi_input[1]);
		close(cgi_output[0]);
	
		//redirction
		printf_debug("begin redirc");
		dup2(cgi_input[0],0);  //????????????????????????
		dup2(cgi_output[1],1);
	//	printf_debug("redirc sucess");
	
		sprintf(method_env,"REQUEST_METHOD=%s",method);
		putenv(method_env);
	//	printf_debug("put method_env sucess");

		if(strcasecmp("GET",method) == 0){
			sprintf(query_env,"QUERY_STRING=%s",query_string);
			putenv(query_env);
	//		printf_debug("put query_env sucess");
		}
		else{//post
			sprintf(content_len_env,"CONTENT_LENGTH=%d",content_length);
			putenv(content_len_env);
	//		printf_debug("put content_len_env success");
		}
		
	//	printf_debug("begin execl....");
		execl(path,path,NULL);//??????????????????????????????????
		//execl(path,NULL);//??????????????????????????????????
		printf("child exit");
		exit(1);
	}
	else{//father
	//	printf_debug("enter father proc");
		close(cgi_input[0]);
		close(cgi_output[1]);
	
		int i = 0;
		char c = '\0';
	
		if(strcasecmp("POST",method) == 0){
			for(;i < content_length;++i){
				recv(sock_client,&c,1,0);
				write(cgi_input[1],&c,1);
			}
		}
	
		while( read(cgi_output[0],&c,1) > 0 ){
			send(sock_client,&c,1,0);
		}
	
		close(cgi_input[1]);
		close(cgi_output[0]);
	
		waitpid(id,NULL,0);
//		printf_debug("father exit");
	}
	
}


void echo_html(int sock,const char *path,unsigned int file_size)
{
	if(!path){
		return;
	}
	int fd = open(path,O_RDONLY);
	if(fd == -1){
		printf_debug("open html error");
		return;
	}
	printf_debug("open html success");

	char echo_line[1024];
	memset(echo_line,'\0',sizeof(echo_line));

	strncpy(echo_line,HTTP_VERSION,strlen(HTTP_VERSION));
	strcat(echo_line," 200 OK");
	strcat(echo_line,"\r\n\r\n");

	send(sock,echo_line,strlen(echo_line),0);
	printf_debug("send echo_line success");

	if(sendfile(sock,fd,NULL,file_size) == -1){
		printf_debug("sendfile error");
		close(fd);
		return;
	}

	printf_debug("sendfile sucess!");
	close(fd);
}


void *accept_request(void *arg)
{
	pthread_detach(pthread_self());
	int sock = (int)arg;
	int cgi = 0;
	int i = 0;
	int j = 0;
	printf("get a connect,,,sock is %d\n",sock);
	
	char buf[COMM_SIZE];
	char method[COMM_SIZE];
	char url[COMM_SIZE];
	char path[COMM_SIZE];
	char *query_string = NULL;
	struct stat st;

	memset(buf,'\0',sizeof(buf));
	memset(method,'\0',sizeof(method));
	memset(url,'\0',sizeof(url));
	memset(path,'\0',sizeof(path));
	
#ifdef _DEBUG_
	while(get_line(sock,buf,sizeof(buf)) > 0){
		printf("%s",buf);
		//fflush(stdout);
	}
#endif
	//get request line
	if(get_line(sock,buf,sizeof(buf))<0){
		return NULL;
	}
	printf("--------------------------------------->%s\n",buf);
	//get method
	while(!isspace(buf[i]) &&\
			i<sizeof(buf) &&\
			j<sizeof(method)-1)
	{
		method[j++] = buf[i++];
	}
	
	printf("before cmp aeecpet->   %s\n",method);

	if(strcasecmp(method,"GET") != 0 &&\
			strcasecmp(method,"POST") != 0)
	{
		echo_error_to_client();
		return (void *)-1;
	}
	
	
	//get url
	while(isspace(buf[i]) && i<sizeof(buf)){//jump out the space  [GET    /index.html]
		++i;
	}

	j = 0;
	while(!isspace(buf[i]) &&\
			i<sizeof(buf) &&\
			j<sizeof(url)-1)
	{
		url[j++] = buf[i++];
	}

	printf("method:%s url:%s\n",method,url);
	
	if(strcasecmp(method,"POST") == 0){ //"POST"
		cgi = 1;
	}
	
	if(strcasecmp(method,"GET") == 0){ //"GET"
		query_string = url;
		while(*query_string != '?' && *query_string != '\0'){
			query_string++;
		}
		if(*query_string == '?'){
			cgi = 1;
			*query_string = '\0';
			query_string++;
		}
	}
	sprintf(path,"htdocs%s",url);
	printf("path :%s\n",path);
	//memset(url,'\0',sizeof(url));//----------------------------------------------------------------------->
	printf("query_string :%s\n",query_string);
		
	if(path[strlen(path)-1] == '/'){
		strcat(path,MAIN_PAGE);
	}
	
	printf("%s\n",path);
	//judge if the file exist

	if(stat(path,&st) == -1){
		//while(get_line(sock,buf,sizeof(buf)) > 0);
		printf_debug("file is not exist!");
		clear_header(sock);
	  //  echo_error_to_client();
	}
	else{
		printf_debug("file is exist");
		if( (st.st_mode & S_IFMT) == S_IFDIR ){
			printf_debug("file is a dirctory");
			strcat(path,"/");
			strcat(path,MAIN_PAGE);
		}
		else if( (st.st_mode & S_IXUSR ||\
					st.st_mode & S_IXGRP ||\
						st.st_mode &S_IXOTH) )
		{
			cgi = 1;
		}	
		else{
			//do nothing
		}
		printf("path :%s\n",path);
		printf("cgi:%d\n",cgi);
		if(cgi == 1){
			printf_debug("begin execute_cgi");
			execute_cgi(sock,path,method,query_string);
		}
		else{
			clear_header(sock);
			printf_debug("enter echo_html\n");
			echo_html(sock,path,st.st_size);			
		}

	}

	close(sock);
	return (void *)0;
}

//success: return sock 
//fail   : exit
int startup(const int port)
{
	int opt = 1;
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		//print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(1);
	}
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port); //port's type is short   host--->net
	local.sin_addr.s_addr = INADDR_ANY;  //bind any ip that is effective in current host
	socklen_t len = sizeof(local);

	if( bind(sock,(struct sockaddr*)&local,len) == -1){
		//print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(2);
	}

	if(listen(sock,BACKLOG) == -1){
		//print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
		exit(3);
	}
	return sock;
}

int main(int argc,char *argv[])
{
	struct sockaddr_in client_sock;
	socklen_t len = sizeof(client_sock);

	if(argc != 2){
		Usage(argv[0]);
		exit(1);
	}
	
	int port = atoi(argv[1]);
	int sock = startup(port);

	while(1){
		int new_sock = accept(sock,(struct sockaddr*)&client_sock,&len);	
		if(new_sock == -1){
			//print_log(__FUNCTION__,__LINE__,errno,strerror(errno));
			continue;
		}
		pthread_t pthread_id;
		int ret = pthread_create(&pthread_id,NULL,accept_request,(void *)(new_sock));
	}
	return 0;
}
