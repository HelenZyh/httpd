/*************************************************************************
	> File Name: debug_cgi.c
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Fri 11 Mar 2016 12:04:10 AM EST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char method[1024];
	char query_string[1024];
	char post_data[4096];
	int content_length = -1;

	memset(method,'\0',sizeof(method));
	memset(query_string,'\0',sizeof(query_string));
	memset(post_data,'\0',sizeof(post_data));

	strcpy(method,getenv("REQUEST_METHOD"));
	if( strcasecmp(method,"GET") == 0 ){
		strcpy(query_string,getenv("QUERY_STRING"));
		printf("query_string:%s</p><br/>",query_string);
	}
	else if( strcasecmp(method,"POST") == 0){
		int content_length = atoi(getenv("CONTENT_LENGTH"));
		int i = 0;
		for(;i<content_length;++i){
			read(0,&post_data[i],1);
		}
		post_data[i] = '\0';
		printf("post_data :%s</p><br/>",post_data);
	}
	else{
		return 1;
	}
	printf("</html>\n");
	return 0;
}
