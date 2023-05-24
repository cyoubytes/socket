#include<fcntl.h>



void fcntl_fd(int fd, bool block)
{
	int flags = fcntl(fd, F_GETFL, 0);
	if (block)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
}
