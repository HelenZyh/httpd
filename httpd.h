/*************************************************************************
	> File Name: httpd.h
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Sat 05 Mar 2016 09:06:33 PM EST
 ************************************************************************/

#ifndef _HTTPD_H_
#define _HTTPD_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include <fcntl.h>

#define BACKLOG 5
#define COMM_SIZE 1024
#define MAIN_PAGE "index.html"
#define HTTP_VERSION "HTTP/1.0"
	
#endif
