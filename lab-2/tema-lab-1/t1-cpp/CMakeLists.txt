cmake_minimum_required(VERSION 3.21)
project(t1_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpthread" )
MATH(EXPR stack_size "16 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,-z,stack-size=${stack_size}")

add_executable(t1_cpp main.cpp src/sequential.cpp src/timer.cpp src/multiThreaded.cpp)
