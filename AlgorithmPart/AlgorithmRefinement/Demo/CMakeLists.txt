cmake_minimum_required(VERSION 3.15)
project(Demo C)

set(CMAKE_C_STANDARD 99)

add_executable(Demo main.c data_structure/list.c data_structure/list.h application/frames.c application/frames.h data_structure/dlist.c data_structure/dlist.h)