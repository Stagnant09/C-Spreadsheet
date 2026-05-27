cmake_minimum_required(VERSION 4.1)
project(txtbsdsp C)

set(CMAKE_C_STANDARD 11)

add_executable(txtbsdsp main.c
        Cell.h
        Functions.h
        Functions.c
        Parser.h
        Parser.c
        utils.h
        utils.c)

if(UNIX)
    target_link_libraries(txtbsdsp m)
endif()
