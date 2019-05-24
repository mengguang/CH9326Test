cmake_minimum_required(VERSION 3.14)
project(CH9326Test C)

set(CMAKE_C_STANDARD 11)

link_directories(lib)
include_directories(lib)
add_executable(CH9326Test main.c lib/CH9326DLL.H)
target_link_libraries(CH9326Test CH9326DLL)