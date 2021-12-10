cmake_minimum_required(VERSION 3.17)
project(hw1_wet_try)

set(CMAKE_CXX_STANDARD 14)

add_executable(hw1_wet_try main.cpp library1.h library1.cpp Player.h trees.h avltree.h node.h
        GroupManager.cpp GroupManager.h)