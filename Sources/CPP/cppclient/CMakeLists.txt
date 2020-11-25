cmake_minimum_required(VERSION 3.17)
project(task1)

set(CMAKE_CXX_STANDARD 14)

add_executable(task1 main.cpp UDP_Client.cpp UDP_Client.h TCP_Client.cpp TCP_Client.h)