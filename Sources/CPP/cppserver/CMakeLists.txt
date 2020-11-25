cmake_minimum_required(VERSION 3.17)
project(server)

set(CMAKE_CXX_STANDARD 14)

add_executable(server main.cpp UDP_Server.cpp UDP_Server.h TCP_Server.cpp TCP_Server.h)