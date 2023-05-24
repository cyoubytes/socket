cmake_minimum_required (VERSION 3.0)

project (server)

add_compile_options(-g -std=c++11)
add_executable(server server.cc)
add_executable(client client.cc)
