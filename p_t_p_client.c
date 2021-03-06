#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
				do{\
					perror(m);\
					exit(EXIT_FAILURE);\
				}while(0)

void client_exit(int sig){
	printf("subprocess exit\n");
	exit(EXIT_SUCCESS);
}

int main(void){
	int sock;
	//创建套接字
	if( (sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0 ){
		ERR_EXIT("socket");
	}

	struct sockaddr_in servaddr;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//连接套接字
	if( connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0 ){
		ERR_EXIT("connect");
	}

	//分出两个进程处理接受跟输入信息
	pid_t pid = fork();

	//父进程处理接受信息
	if( pid != 0 ){
 		char recvbuf[1024];
    //死循环接受信息
	 	while(1){
			//清空recvbuf
      memset(recvbuf,0,sizeof(recvbuf));
			//读取另一方的会话信息写进recvbuf
      int ret = read(sock, recvbuf, sizeof(recvbuf));
      //判断另一方是否断开
      if( ret == 0 ){ 
        printf("Client close connection\n");
				//发送信号通知子进程,0是通知所有子进
				kill(0,SIGUSR1);
        break;  
      }       
      //传输错误
      if( ret == -1 ){
        printf("Message translation error\n");
      }       
      fputs(recvbuf,stdout);
 		}
		close(sock);
	}else{
		//等待父级进程信息
		signal(SIGUSR1,client_exit);
		//子进程负责发送信息
		char sendbuf[1024] = {0};
		//死循环接受信息
		while(1){
			while( fgets(sendbuf, sizeof(sendbuf),stdin) != NULL ){
				write(sock,sendbuf,strlen(sendbuf));
				memset(sendbuf,0,sizeof(sendbuf));
			}
		}
		close(sock);
	}




	return 0;

}



