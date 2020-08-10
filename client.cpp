#include"sock_util.h"


int main(int argc, char ** argv)
{

	const char * ip = "127.0.0.1";
	short port = 8888;
	bool nonblock = argv[1];
	int conn = SockUtil::Connect(ip, port, 5, nonblock);
	if(conn > 0)
	{
		char buf[256];
		cin>>buf;
		send(conn, buf, 256, 0);
		std::cout<<buf<<std::endl;
		recv(conn, buf, 256, 0);
		std::cout<<buf<<std::endl;
		close(conn);
	}
}
