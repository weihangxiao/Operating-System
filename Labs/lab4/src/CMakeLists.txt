cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp operation.h scheduler.cpp scheduler.h)