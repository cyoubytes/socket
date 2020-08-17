#include"sock_util.h"

int main(void)
{

	const char * ip="127.0.0.1";
	short port=8888;
    int sockfd=SockUtil::Listen(ip,port);
	SockUtil::Accept(sockfd);
	return 0;
}
