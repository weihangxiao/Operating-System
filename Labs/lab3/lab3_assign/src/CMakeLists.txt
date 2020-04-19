cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp process.cpp process.h PTE.h frame.cpp frame.h pager_algo.cpp pager_algo.h page_fault_handler.cpp page_fault_handler.h)