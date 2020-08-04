#include"sock_util.h"

int SockUtil::Connect(const char* ip, unsigned short port, int connect_timeout_ms, bool nonblock)
{
	if(INADDR_NONE == inet_addr(ip))
	{
		LOG("Invalid ip");
		return false;
	}
	
	struct sockaddr_in seraddr;
	bzero(&seraddr, 0);
	seraddr.sin_family = AF_INET;
	seraddr.sin_addr.s_addr = inet_addr(ip);
	seraddr.sin_port = htons(port);	

	int sockfd=-1;
	if(nonblock)
		sockfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
	else
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		LOG("Socket Failed");
		return -1;
	}
	
	int error = 0;
	int connect_ret = connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));

	if(nonblock)
	{
		fd_set wset;
		FD_ZERO(&wset);
		struct timeval tval;
		tval.tv_sec = 0;
		tval.tv_usec = connect_timeout_ms;
		FD_SET(sockfd, &wset);
		if(connect_ret != 0 && (errno != EINPROGRESS) && (errno != EAGAIN))
		{
			LOG("connect failed");
			error = -1;
		}
		if( error ==0 && connect_ret !=0 )
		{
			connect_ret = select(sockfd+1, nullptr, &wset ,nullptr, &tval);
			if( connect_ret <= 0 ) // timeout or failed
			{
				LOG("connect timeout");
				close(sockfd);
				return  -1;
			}
			if(FD_ISSET(sockfd, &wset))
			{
				socklen_t errlen = sizeof(error);
				if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &errlen) < 0)
				{
					LOG("getsockopt failed");
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
			close(sockfd);
			return -1;
		}
	}
	else
	{
		if(connect_ret != 0)
		{
			LOG("connect block failed");
			close(sockfd);
			return -1;
		}
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
		LOG("Setsockopt SO_REUSEADDR failed");
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
			LOG("inet_addr failed");
			close(sockfd);
			return -1;
		}
	}

	if(bind(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0 )
	{
		LOG("bind failed");
		close(sockfd);
		return -1;
	}

	if(listen(sockfd, 256) < 0)
	{
		LOG("listen failed");
		close(sockfd);
		return -1;
	}
	return sockfd;
}

int SockUtil::Accept(int listenfd)
{
	 struct sockaddr_in cliaddr;
	 socklen_t clilen = sizeof(cliaddr);
	 int conn=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
	 cout<<"listenfd="<<listenfd<<endl;
	 char buf[1024];
	 recv(conn, buf, 1024, 0);
	 std::cout<<buf<<std::endl;
	 send(conn, buf, 1024, 0);
	 return 0;
}

