cmake_minimum_required(VERSION 3.25)
project(RPG C)

set(CMAKE_C_STANDARD 23)

add_executable(RPG main.c system/StartGame.h system/StartGame.c system/input.h system/input.c persons/characters.h persons/characters.c Items/things.h Items/things.c map/location.h map/location.c)
