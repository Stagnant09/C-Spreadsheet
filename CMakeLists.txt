cmake_minimum_required(VERSION 3.15)
project(txtbsdsp C)

set(CMAKE_C_STANDARD 11)

# Enable optimisations so the compiler can work alongside the inline asm
set(CMAKE_C_FLAGS_RELEASE "-O2 -march=native")

add_executable(txtbsdsp
    main.c
    Cell.h
    Functions.h
    Functions.c
    Parser.h
    Parser.c
    utils.h
    utils.c
)

if(UNIX)
    target_link_libraries(txtbsdsp m)
endif()
