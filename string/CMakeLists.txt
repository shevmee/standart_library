cmake_minimum_required(VERSION 3.10)
project(BasicString)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# cmake .. -DCMAKE_BUILD_TYPE=Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -g")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address,undefined")

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
        set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=memory")
    endif()

    message(STATUS "Sanitizers enabled: AddressSanitizer and UndefinedBehaviorSanitizer")
else()
    message(STATUS "Sanitizers are only enabled in Debug mode")
endif()

include_directories(include/)

add_subdirectory(third_party/googletest)

add_executable(BasicString main.cpp)

add_executable(BasicStringTests test_main.cpp)

target_link_libraries(BasicStringTests gtest gtest_main pthread)
