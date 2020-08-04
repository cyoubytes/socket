#ifndef UTIL_H_H
#define UTIL_H_H

#include<stdarg.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
#include<errno.h>
#include<signal.h>
#include<stdarg.h>
#include<sys/select.h>
#include<fcntl.h>
#include<pthread.h>
#include<iostream>

using namespace std;

#define LOG(msg) \
do \
{ \
	log("[file %s]:[line %d]:[func %s] %s \n", __FILE__, __LINE__, __func__, msg); \
} while(0)



void log(const char* msg, ...);

#endif
