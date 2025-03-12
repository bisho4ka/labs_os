cmake_minimum_required(VERSION 3.10)
project(lab5-7_tests)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(googletest)

# Поиск ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Поиск потоков (pthread)
find_package(Threads REQUIRED)

# Добавление исполняемого файла для тестов
add_executable(lab5-7_test
    lab5-7_test.cpp
    ../lab5-7/src/tcontroller.cpp
    ../lab5-7/src/tmessaging.cpp
    ../lab5-7/src/ttopology.cpp
    ../lab5-7/src/tworker.cpp
)

# Подключение библиотек
target_link_libraries(lab5-7_test PRIVATE gtest_main ${ZMQ_LIBRARIES} Threads::Threads)

# Подключение заголовочных файлов
target_include_directories(lab5-7_test PRIVATE ../lab5-7/include)

# Добавление тестов
add_test(NAME lab5-7_test COMMAND lab5-7_test)