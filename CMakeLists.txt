cmake_minimum_required(VERSION 4.0)
project(Library)

set(CMAKE_CXX_STANDARD 14)

add_executable(Library
        main.cpp
        library.h
        library.cpp
)
