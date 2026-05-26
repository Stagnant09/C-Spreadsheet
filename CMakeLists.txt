cmake_minimum_required(VERSION 4.1)
project(txtbsdsp C)

set(CMAKE_C_STANDARD 11)

add_executable(txtbsdsp main.c
        Cell.h
        Functions.h
        Functions.c)
