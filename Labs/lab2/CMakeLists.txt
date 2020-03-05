cmake_minimum_required(VERSION 3.7)
project(lab2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lab2_assign/gradeit.sh
        lab2_assign/input0
        lab2_assign/input1
        lab2_assign/input2
        lab2_assign/input3
        lab2_assign/input4
        lab2_assign/input5
        lab2_assign/input6
        lab2_assign/outformat.c
        lab2_assign/README.txt
        lab2_assign/refout.tar.Z
        lab2_assign/refout_v.tar.Z
        lab2_assign/rfile
        lab2_assign/round.c
        lab2_assign/runit.sh scheduler.cpp scheduler.h lab2_assign/src/process.cpp lab2_assign/src/process.h lab2_assign/src/event.cpp lab2_assign/src/event.h lab2_assign/src/scheduler.cpp lab2_assign/src/scheduler.h main.cpp lab2_assign/src/DESLayer.cpp lab2_assign/src/DESLayer.h)

add_executable(lab2 ${SOURCE_FILES})