#pragma once

#include<stdarg.h>
#include <iostream>



void log(const char * msg, ...){	

	va_list valist;
	va_start(valist, msg);
	vprintf(msg, valist);
	va_end(valist);
}

/**
template<typename T0>
void log(T0 value) {
    std::cout << value <<" ";
}
template<typename T, typename... Ts>
void log(T value, Ts... args) {
    std::cout << value <<" ";
    log(args...);
}
*/

#define LOG(msg) \
do \
{\
	log("[file %s]:[line %d]:[func %s] %s \n", __FILE__, __LINE__, __func__, msg);  \
}while(0)

