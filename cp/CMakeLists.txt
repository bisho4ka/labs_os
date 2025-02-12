cmake_minimum_required(VERSION 3.10)
project(battleships)

set(CMAKE_CXX_STANDART 20)

add_executable(battleships server.cpp lib/GameManager.cpp lib/utils.cpp)
add_executable(client client.cpp lib/GameManager.cpp lib/utils.cpp)