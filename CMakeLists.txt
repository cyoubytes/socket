cmake_minimum_required (VERSION 3.0)

project (server)

add_compile_options(-g -std=c++11)
add_executable(server server.cpp sock_util.cpp util.cpp)
add_executable(client client.cpp sock_util.cpp util.cpp)
