cmake_minimum_required(VERSION 3.21)
project(t2_cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -lpthread" )

add_executable(t2_cpp src/main.cpp src/multiThreaded.cpp src/timer.cpp src/barrier.h)
