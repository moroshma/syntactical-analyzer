cmake_minimum_required(VERSION 3.26)
project(syntactical_analyzer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(syntactical_analyzer
        handler.cpp
        handler.h
        parser.cpp
        parser.h)
