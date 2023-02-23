cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_library(MathParser
    Token.h
    Token.cpp

    MathExp.h
    MathExp.cpp
)

target_include_directories(MathParser PUBLIC .)
