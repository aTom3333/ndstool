cmake_minimum_required(VERSION 3.8)
project(ndstool)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


file(GLOB_RECURSE SOURCE_FILES source/*)

add_executable(ndstool ${SOURCE_FILES})