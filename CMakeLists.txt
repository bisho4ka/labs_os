cmake_minimum_required(VERSION 3.10)
project(cp_project)

set(CMAKE_CXX_STANDARD 17)

# Добавляем поддиректории
add_subdirectory(cp)
add_subdirectory(tests)