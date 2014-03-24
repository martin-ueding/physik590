cmake_minimum_required(VERSION 2.6)
project(Bachelorarbeit)

set(CMAKE_CXX_FLAGS "-O3 -pedantic -std=c++11 -Wall")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_executable(hello main-hello.cpp)
target_link_libraries(hello boost_program_options)
