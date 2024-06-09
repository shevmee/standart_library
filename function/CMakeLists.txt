cmake_minimum_required(VERSION 3.28)
project(function)

set(CMAKE_CXX_STANDARD 20)

set(GTEST_DIR /home/shevmee/CLionProjects/googletest)

# add_executable(function basic_usage.cpp function.h)

set(GTEST_BUILD_DIR ${CMAKE_BINARY_DIR}/googletest)

file(MAKE_DIRECTORY ${GTEST_BUILD_DIR})

add_subdirectory(${GTEST_DIR} ${GTEST_BUILD_DIR})

add_executable(test_app test_main.cpp)

target_link_libraries(test_app gtest gtest_main)

enable_testing()

# Добавьте тесты
add_test(NAME RunAllTests COMMAND test_app)
