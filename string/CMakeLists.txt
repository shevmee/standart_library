cmake_minimum_required(VERSION 3.10)
project(BasicString)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party/googletest)

add_executable(BasicString main.cpp)

add_executable(BasicStringTests test_main.cpp)

target_link_libraries(BasicStringTests gtest gtest_main pthread)
