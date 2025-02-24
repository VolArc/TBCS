cmake_minimum_required(VERSION 3.30)
project(TBCS)

set(CMAKE_CXX_STANDARD 26)

add_executable(TBCS main.cpp
        interface.h
        BinaryTree.h
        List.h
        Array.h)
