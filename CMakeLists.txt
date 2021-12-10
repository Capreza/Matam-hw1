cmake_minimum_required(VERSION 3.17)
project(hw1_wet)

set(CMAKE_CXX_STANDARD 11)

add_executable(i_really_try main.cpp library1.cpp library1.h GroupManager.h GroupManager.cpp trees.h subLibrary.h
        avltree.h node.h)