#ifndef SOCK_UTIL_H
#define SOCK_UTIL_H

#include"util.h"

class SockUtil
{
	public:
		static int Connect(const char* ip, unsigned short port);
		static int Connect_Nonblock(const char* ip, unsigned short port, int connect_timeout_ms);
		static int Listen(const char* ip, unsigned short port);
		static int Accept(int listenfd);
};


#endif
