cmake_minimum_required(VERSION 3.2)

PROJECT(Jason)

set(CMAKE_CXX_STANDARD 11)

add_executable(Jason Jason.c scan.c symbol.c object.c quad.c scan.h symbol.h quad.h object.h)

