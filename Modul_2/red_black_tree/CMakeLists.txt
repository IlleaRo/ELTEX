cmake_minimum_required(VERSION 3.25)
project(red_black_tree C)

set(CMAKE_C_STANDARD 23)

add_executable(red_black_tree main.c BRTree/tree.h BRTree/tree.c system/input.h system/input.c)
