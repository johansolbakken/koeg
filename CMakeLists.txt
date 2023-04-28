cmake_minimum_required(VERSION 3.15)

project(Koeg)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/fmt)

add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(Koeg ${SOURCES})
target_include_directories(Koeg PRIVATE src vendor/glfw/include vendor/glad/include vendor/fmt/include)
target_link_libraries(Koeg glfw glad fmt)