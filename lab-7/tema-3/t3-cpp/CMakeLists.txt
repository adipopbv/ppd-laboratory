cmake_minimum_required(VERSION 3.21)
project(t3_cpp)

set(CMAKE_CXX_STANDARD 11)

#add_executable(t3_cpp sequential.cpp classic-mpi.cpp scatter-mpi.cpp)
add_executable(t3_cpp sequential.cpp timer.cpp)
