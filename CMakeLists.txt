cmake_minimum_required(VERSION 3.2)

PROJECT(Jason)

set(CMAKE_CXX_STANDARD 11)

add_executable(
	Jason 
	Jason.c 
	scan.c 
	symbol.c 
	quad.c 
	object.c
	object.h
	scan.h
	symbol.h 
	quad.h
	)

