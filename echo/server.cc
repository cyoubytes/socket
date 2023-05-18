#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include"config.h"


void echo(int connfd){
	for(;;) {
		
		std::cout<<"echo start"<<std::endl;
		char buffer[BUFFER] = {0};
		int readn = recv(connfd, buffer, BUFFER, 0);
		std::cout<<"readn"<<readn<<std::endl;
		if(readn < 0) {
			std::cout<<"rcv failed"<<std::endl;
			break;
		}
		else if(readn == 0){
			std::cout<<"client closed"<<std::endl;
			break;
		}

		int writen = send(connfd, buffer, sizeof(buffer), 0);
		if(writen < 0){
			std::cout<<"send failed"<<std::endl;
			break;
		}
	}
}

int main(int argc, char ** argv ){

	char buffer[BUFFER];

	struct sockaddr_in server_addr;
	bzero(&server_addr, 0);
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if(*ip!='\0'){
		
		//server_addr.sin_addr.s_addr = inet_addr(ip);
		inet_pton(AF_INET, ip, &server_addr.sin_addr);

		if( INADDR_NONE == server_addr.sin_addr.s_addr){
			std::cout<<"inet_addr failed"<<std::endl;
		}
	}

	int sockfd = socket(AF_INET, SOCK_STREAM ,0);
	if(sockfd < 0){
		std::cout<<"scoket failed"<<std::endl;
		return -1;
	}

	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		std::cout<<"bind failed"<<std::endl;
		close(sockfd);
		return -1;
	}

	if(listen(sockfd, BACKLOG)<0){
		std::cout<<"listen failed"<<std::endl;
		close(sockfd);
		return -1;
	}

	struct sockaddr_in accept_addr;
	socklen_t accept_len = sizeof(accept_addr);

	for(;;){
		int accept_fd = accept(sockfd, (struct sockaddr *)& accept_addr, &accept_len);
		if(accept_fd > 0){
			echo(accept_fd);
		}
		else{
			std::cout<<"accept failed"<<std::endl;
			continue;
		}
	}

	close(sockfd);
	return 0;
}

