#include"sock_util.h"

int SockUtil::Connect(const char* ip, unsigned short port)
{
	if(INADDR_NONE == inet_addr(ip))
	{
		LOG("Invalid Ip");
		return false;
	}
	
	struct sockaddr_in seraddr;
	bzero(&seraddr, 0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr(ip);
	seraddr.sin_port = htons(port);	

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		LOG("Socket Failed");
		return -1;
	}
	
	int error = 0;
	int connect_ret = connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if(connect_ret != 0)
	{
		LOG("Connect Block Failed");
		close(sockfd);
		return -1;
	}
	return sockfd;
}
int SockUtil::Connect_Nonblock(const char* ip, unsigned short port, int connect_timeout_sec)
{
	if(INADDR_NONE == inet_addr(ip))
	{
		LOG("Invalid Ip");
		return false;
	}
	
	struct sockaddr_in seraddr;
	bzero(&seraddr, 0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr(ip);
	seraddr.sin_port = htons(port);	

	int sockfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
	if(sockfd < 0)
	{
		LOG("Socket Failed");
		return -1;
	}
	
	int error = 0;
	int connect_ret = connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

	fd_set wset;
	FD_ZERO(&wset);
	struct timeval tval;
	tval.tv_sec = connect_timeout_sec;
	tval.tv_usec = 0;
	FD_SET(sockfd, &wset);
	if(connect_ret != 0 && (errno != EINPROGRESS) && (errno != EAGAIN))
	{
		LOG("Connect Failed");
		error = -1;
	}
	if( error ==0 && connect_ret !=0 )
	{
		connect_ret = select(sockfd+1, nullptr, &wset ,nullptr, &tval);
		if( connect_ret <= 0 ) // timeout or failed
		{
			LOG("Connect Timeout");
			close(sockfd);
			return  -1;
		}
		if(FD_ISSET(sockfd, &wset))
		{
			socklen_t errlen = sizeof(error);
			if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen) < 0)
			{
				LOG("Getsockopt Failed");
				error = -1;
			}
		}
		else
		{
			error = -1;
		}
	}
	if( error !=0 )
	{
		LOG("Connect_Nonblock Failed");
		close(sockfd);
		return -1;
	}
	return sockfd;
}


int SockUtil::Listen(const char* ip, unsigned short port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM ,0);
	if(sockfd < 0)
	{
		LOG("Socket Failed");
		return -1;
	}
	int reuse = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
	{
		LOG("Setsockopt SO_REUSEADDR Failed");
	}

	struct sockaddr_in seraddr;
    bzero(&seraddr, 0);
	seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr= INADDR_ANY;
	seraddr.sin_port = htons(port);

	if(*ip !='\0')
	{
		seraddr.sin_addr.s_addr = inet_addr(ip);
		if(INADDR_NONE == seraddr.sin_addr.s_addr)
		{
			LOG("Inet_addr Failed");
			close(sockfd);
			return -1;
		}
	}

	if(bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0 )
	{
		LOG("Bind Failed");
		close(sockfd);
		return -1;
	}


	if(listen(sockfd, 256) < 0)
	{
		LOG("Listen Failed");
		close(sockfd);
		return -1;
	}
	return sockfd;
}

int SockUtil::Accept(int listenfd)
{
	 struct sockaddr_in cliaddr;
	 socklen_t clilen = sizeof(cliaddr);


	 for(;;)
	 {
		 pid_t pid;
		 int conn=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen); 
		 if(conn <0)
		 {
			if(errno != EAGAIN || errno != EWOULDBLOCK)
			{
				 LOG("accept failed");
				 continue;
			}
		 }

		 if((pid=fork()) ==0)
		 {
			close(listenfd);
			echoServer(conn);
			shutdown(conn,2);
			//close(conn);
			exit(0);
		 }

		sleep(10);
		close(conn);
	}

	close(listenfd);
	return 0;
}


