#include"util.h"


void log(const char* msg, ...)
{
    va_list vlist;
    va_start(vlist,msg);
    vprintf(msg,vlist);
    va_end(vlist);
}

void echoServer(int connfd)
{
	for(;;)
	{
        char buf[1024]={0};
		int readn = recv(connfd, buf, 1024,0);
		if(readn < 0)
		{
			LOG("read failed");
			break;
		}
		else if(readn == 0)
		{
			LOG("client closed");
			break;
		}
		int writen = send(connfd, buf, strlen(buf), 0);
		if(writen < 0)
		{
			LOG("write failed");
			break;
		}
	}
}


