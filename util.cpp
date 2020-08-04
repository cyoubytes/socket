#include"util.h"


void log(const char* msg, ...)
{
    va_list vlist;
    va_start(vlist,msg);
    vprintf(msg,vlist);
    va_end(vlist);
}

