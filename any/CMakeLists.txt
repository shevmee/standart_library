cmake_minimum_required(VERSION 3.28)
project(any)

set(CMAKE_CXX_STANDARD 20)

add_executable(any any.cpp any.h)

set(GTEST_DIR /home/shevmee/CLionProjects/googletest)

set(GTEST_BUILD_DIR ${CMAKE_BINARY_DIR}/googletest)

file(MAKE_DIRECTORY ${GTEST_BUILD_DIR})

add_subdirectory(${GTEST_DIR} ${GTEST_BUILD_DIR})

add_executable(test_app test_main.cpp)
target_link_libraries(test_app gtest_main)



