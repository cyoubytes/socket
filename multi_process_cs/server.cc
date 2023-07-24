#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <sys/wait.h>
#include "config.h"

typedef void(sigfunc)(int);

void sighandler(int signo,sigfunc * func)
{
    struct sigaction sa; 
    sa.sa_flags = SA_RESTART;
    sa.sa_handler=func;
    sigaction(signo,&sa,NULL);
}

void sig_child(int signo)
{
    pid_t pid;
    int stat;
    while(pid=wait(&stat)>0)
    //while((pid=waitpid(-1,&stat,WNOHANG))>0)
        std::cout<<"child terminated "<<pid<<std::endl;
    return;
}


int main(int argc, char **argv) {
	struct sockaddr_in server_addr;
	bzero(&server_addr, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (*ip != '\0') {
		// server_addr.sin_addr.s_addr = inet_addr(ip);
		inet_pton(AF_INET, ip, &server_addr.sin_addr);

		if (INADDR_NONE == server_addr.sin_addr.s_addr) {
			std::cout << "inet_addr failed" << std::endl;
		}
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		std::cout << "socket failed" << std::endl;
		return -1;
	}

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
	    0) {
		std::cout << "bind failed" << std::endl;
		close(sockfd);
		return -1;
	}

	if (listen(sockfd, BACKLOG) < 0) {
		std::cout << "listen failed" << std::endl;
		close(sockfd);
		return -1;
	}

	struct sockaddr_in accept_addr;
	socklen_t accept_len = sizeof(accept_addr);
	int accept_fd;
	pid_t pid;

	sighandler(SIGCHLD, sig_child);

	for (;;) {
		if ((accept_fd = accept(sockfd, (struct sockaddr *)&accept_addr,
					&accept_len)) < 0) {
			if (errno != EINTR)
				std::cout << "accept failed" << std::endl;
			else
				continue;
		}
		if (pid = fork() == 0) {
			close(sockfd);
			char buf[BUFFER] = {};
			for (;;) {
				int readn = recv(accept_fd, buf, BUFFER, 0);
				std::cout << "readn" << readn << std::endl;
				if (readn < 0) {
					std::cout << "rcv failed" << std::endl;
					break;
				} else if (readn == 0) {
					std::cout << "client closed"
						  << std::endl;
					break;
				}

				int writen =
				    send(accept_fd, buf, sizeof(buf), 0);
				if (writen < 0) {
					std::cout << "send failed" << std::endl;
					break;
				}
			}
			exit(0);
		} else
			close(accept_fd);
	}

	return 0;
}

