#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void receive_message(int conn){
	char recvbuf[1024];
	while(1){
		memset(recvbuf,0,sizeof(recvbuf));
		int ret = read(conn, recvbuf, sizeof(recvbuf));
		fputs(recvbuf,stdout);
		write(conn, recvbuf, ret);
	}
}

void send_message(){
	echo 1;
}

void handler(int sig){
  print("receive the server signal,exit");
	exit(); 
}
