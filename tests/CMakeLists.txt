cmake_minimum_required(VERSION 3.10)
project(cp_tests)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Поиск потоков (pthread)
find_package(Threads REQUIRED)

# Добавление исполняемого файла для тестов
add_executable(cp_tests
    cp_test.cpp
    ../cp/src/server.cpp
    ../cp/src/client.cpp
    ../cp/src/game.cpp
)

# Подключение библиотек
target_link_libraries(cp_tests PRIVATE gtest_main Threads::Threads)

# Подключение заголовочных файлов
target_include_directories(cp_tests PRIVATE ../cp/include)

# Добавление тестов
add_test(NAME cp_tests COMMAND cp_tests)