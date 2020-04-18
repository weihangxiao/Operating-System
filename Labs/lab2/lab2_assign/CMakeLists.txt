cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(src
        DESLayer.cpp
        DESLayer.h
        event.cpp
        event.h
        main.cpp
        process.cpp
        process.h
        scheduler.cpp
        scheduler.h)
