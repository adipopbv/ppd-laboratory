cmake_minimum_required(VERSION 3.22)
project(tema_p2 CUDA)

set(CMAKE_CUDA_STANDARD 14)

add_executable(tema_p2 filter.cu)

set_target_properties(tema_p2 PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
