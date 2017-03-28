#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//下面三个头文件为本练习重要部分
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define BUFFER_SIZE 256
#define PORT 4568
char recv_buff[BUFFER_SIZE];
char send_buff[BUFFER_SIZE];
int main(void)
{
	char write_flag=0;
	char read_flag=1;
	int sockfd;//套结字
	struct sockaddr_in cli_addr;  //服务器的IPv4的套结字结构
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)//创建套结字
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("connect server success.\n");
	}
	bzero(&cli_addr,sizeof(struct sockaddr_in));
	cli_addr.sin_family=AF_INET;
	cli_addr.sin_port=htons(PORT);
	cli_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(connect(sockfd,(struct sockaddr*)(&cli_addr),sizeof(struct sockaddr))<0)
	{
		perror("connect");
		exit(2);
	}
	else
	{
		printf("connect server success.\n");
	}
	memset(send_buff,0x00,sizeof(send_buff));
	memset(recv_buff,0x00,sizeof(send_buff));
	while(1)
	{
		if(strlen(send_buff)==0&&write_flag==1)
		{
			fgets(send_buff,BUFFER_SIZE,stdin);
			if(write(sockfd,send_buff,strlen(send_buff))<0)
			{
				perror("write");
				exit(3);
			}
			else
			{
				printf("Client:\n");
				printf("	%s\n",send_buff);

			}
			read_flag=1;
			write_flag=0;
			memset(recv_buff,0x00,strlen(recv_buff));

		}
		else if(strlen(recv_buff)==0&&read_flag==1)
		{
			if(read(sockfd,recv_buff,sizeof(recv_buff))<0)
			{
				perror("read");
				exit(4);
			}
			else if(strlen(recv_buff)>0)
			{
				printf("server:\n");
				printf("		:%s\n",recv_buff);
				memset(send_buff,0x00,strlen(send_buff));
				write_flag=1;
				read_flag=0;

			}
		}
	}
	printf("end.\n");
	close(sockfd);   //关闭连接
	return 0;
}
