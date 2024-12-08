cmake_minimum_required(VERSION 3.28)
project(type_traits)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(type_traits main.cpp
        type_traits.h)
