cmake_minimum_required(VERSION 3.16)
project(366_c_homework C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../src)

add_executable(c_homework homework.cpp)
target_link_libraries(c_homework gtest gtest_main)
