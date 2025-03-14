cmake_minimum_required(VERSION 3.10)
project(cp_tests)

set(CMAKE_CXX_STANDARD 17)

# Подключаем GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Указываем исходные файлы
set(SOURCES
    cp_test.cpp
    ../cp/src/client.cpp
    ../cp/src/server.cpp
)

# Создаем исполняемый файл для тестов
add_executable(cp_test ${SOURCES})

# Подключаем GoogleTest к целевому объекту
target_link_libraries(cp_test PRIVATE gtest_main)

# Указываем пути к заголовочным файлам
target_include_directories(cp_test PRIVATE ../cp/include)