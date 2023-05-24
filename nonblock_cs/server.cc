#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<sys/select.h>
#include"config.h"
#include"log.h"
#include"util.h"



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
			LOG("inet_addr failed");
		}
	}

	int sockfd = socket(AF_INET, SOCK_STREAM ,0);
	if(sockfd < 0){
		LOG("scoket failed");
		return -1;
	}

	int flag = 1, ret = 0;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &flag, sizeof(flag)) < 0){
		LOG("failed to set setsock to reuseaddr!");
		ret = -1;
	}


	if(0 == ret){
		if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
			LOG("bind failed");
			ret = -1;
		}
	}
	if(0 == ret){
		if(listen(sockfd, BACKLOG)<0){
			LOG("listen failed");
			ret = -1;
		}
	}

	if( 0 != ret){
		close(sockfd);
		return -1;
	}
	
	fcntl_fd(sockfd, true);
	
	struct sockaddr_in accept_addr;
	socklen_t accept_len = sizeof(accept_addr);
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);

	for(;;){
		int accept_fd = accept(sockfd, (struct sockaddr *)& accept_addr, &accept_len);
		if(accept_fd < 0 ){
			LOG(strerror(errno));
			if( errno != EAGAIN && errno!= EWOULDBLOCK){
				LOG("accespt failed");
			}
			if(select(sockfd+1, &rset, nullptr, nullptr, nullptr) >0){
				accept_fd = accept(sockfd, (struct sockaddr *)& accept_addr, &accept_len);
			}
			else{
				LOG("accespt failed");
			}
		}

		echo(accept_fd);

	}
	close(sockfd);
	return 0;
}

