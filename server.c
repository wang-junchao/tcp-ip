#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int sockfd,newsockfd;
	socklen_t addr_len;	
	struct sockaddr_in ser_addr;
	addr_len=sizeof(struct sockaddr_in);
	if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("creat socket success id:%d\n",sockfd);
	}
	bzero(&ser_addr,sizeof(struct sockaddr_in));
	ser_addr.sin_family=AF_INET;
	ser_addr.sin_port=htons(PORT);
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(bind(sockfd,(struct sockaddr *)(&ser_addr),sizeof(struct sockaddr))<0)
	{
		perror("bind");
		exit(2);
	}
	else
	{
		printf("nind success.\n");
	}
	if(listen(sockfd,10)<0)
	{
		perror("listen");
		exit(3);
	}
	else
	{
		printf("listening...\n");

	}
	memset(send_buff,0x00,sizeof(send_buff));
	memset(recv_buff,0x00,sizeof(send_buff));
	if((newsockfd=accept(sockfd,(struct sockaddr*)(&ser_addr),&addr_len))<0)
	{
		perror("accept");
		exit(4);
	}
	else
	{
		printf("connect success. \n");
	}
	write(newsockfd,"have connect!",strlen("have connect"));
	while(1)
	{
		if(strlen(send_buff)==0&&write_flag==1)
		{
			fgets(send_buff,BUFFER_SIZE,stdin);
			if(write(newsockfd,send_buff,strlen(send_buff))<0)
			{
				perror("write");
				exit(3);
			}
			else
			{
				printf("Client:\n");
				printf("		%s\n",send_buff);
			}
			read_flag=1;
			write_flag=0;
			memset(recv_buff,0x00,strlen(recv_buff));
		}
		else if(strlen(recv_buff)==0&&read_flag==1)
		{
			if(read(newsockfd,recv_buff,sizeof(recv_buff))<0)
			{
				perror("read");
				exit(4);
			}
			else if(strlen(recv_buff)>0)
			{
				printf("Server:\n");
				printf("		: %s\n",recv_buff);
				memset(send_buff,0x00,strlen(send_buff));
				write_flag=1;
				read_flag=0;
			}
		}
	}
	close(sockfd);
	return 0;
}
