cmake_minimum_required(VERSION 3.8)
project(OS)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES memory.c test.c mmemory.h)
add_executable(OS ${SOURCE_FILES})