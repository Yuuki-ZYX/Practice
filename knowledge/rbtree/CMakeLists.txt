cmake_minimum_required(VERSION 3.28)
project(rbtree)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rbtree
        main.cpp
        rbtree.cpp
        rbtree.h)

add_executable(test
        test.cpp
        rbtree.cpp
        rbtree.h)