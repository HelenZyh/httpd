/*************************************************************************
	> File Name: demo_client.c
	> Author: zyh_helen
	> Mail: 2897720955@qq.com 
	> Created Time: Sat 05 Mar 2016 09:37:14 PM EST
 ************************************************************************/

#include"demo_client.h"


void Usage(const char *proc)
{
	printf("%s [IP] [PORT]\n",proc);
}
int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}
	
	int port = atoi(argv[2]);
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1){
		perror("socket");
		exit(1);
	}

	struct sockaddr_in sock_server;
	sock_server.sin_family = AF_INET;
	sock_server.sin_port = htons(port);
	sock_server.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sock,(struct sockaddr*)&sock_server,sizeof(sock_server)) == -1){
		perror("connect");
		exit(1);
	}



	return 0;
}

