#include<iostream>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include"config.h"


int main(int argc, char ** argv){
	
	
	if(INADDR_NONE == inet_addr(ip)){
		std::cout<<"inavlid ip"<<std::endl;
		return -1;
	}

	struct sockaddr_in client_addr;             
	bzero(&client_addr, 0);                     
	client_addr.sin_family = AF_INET;           
	client_addr.sin_addr.s_addr = inet_addr(ip);
	client_addr.sin_port = htons(port);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		std::cout<<"socker failed"<<std::endl;
		return -1;
	}

	if(connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == 0){
		std::cout<<"start"<<std::endl;
		char buffer[BUFFER];
		while(fgets(buffer, sizeof(buffer), stdin) !=nullptr){
			int writen = send(sockfd, buffer, sizeof(buffer), 0);
			if(writen < 0){
				std::cout<<"send failed"<<std::endl;
				break;
			}

			int readn = recv(sockfd, buffer, BUFFER, 0);
			if(readn < 0 ){
				std::cout<<"recv failed"<<std::endl;
				break;
			}
			else if(readn == 0){
				std::cout<<"peer connection closed"<<std::endl;
			}
			else{
				std::cout<<buffer;
			}
		}
		close(sockfd);
	}
}

