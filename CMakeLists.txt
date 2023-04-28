cmake_minimum_required(VERSION 3.15)

project(Koeg)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Koeg ${SOURCES})

