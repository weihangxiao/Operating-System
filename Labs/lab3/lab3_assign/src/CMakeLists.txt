cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 14)

add_executable(src main.cpp process.cpp process.h PTE.h Pager_Algo.cpp Pager_Algo.h frame.cpp frame.h)