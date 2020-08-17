#include"sock_util.h"


int main(int argc, char ** argv)
{

	const char * ip = "127.0.0.1";
	short port = 8888;
	bool nonblock = argv[1];
	int conn = SockUtil::Connect(ip, port);
	if(conn > 0)
	{
		char buf[256];
		while(fgets(buf,sizeof(buf), stdin)!=nullptr)
		{
			int writen=send(conn,buf,strlen(buf),0);
			if(writen<0)
			{
				LOG("write failed");
				break;
			}
			int readn=recv(conn,buf,sizeof(buf),0);
			if(readn<0)
			{
				LOG("read failed");
				break;
			}
			else if(readn==0)
				LOG("peer connection closed");
			else
				cout<<buf;
		}

		close(conn);
	}
}
