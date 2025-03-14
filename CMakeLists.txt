cmake_minimum_required(VERSION 3.10)
project(battleships)

set(CMAKE_CXX_STANDARD 20)

# Указываем путь к заголовочным файлам
include_directories(${CMAKE_SOURCE_DIR}/include)

# Добавляем исходные файлы
add_executable(battleships server.cpp src/utils.cpp src/GameManager.cpp)
add_executable(client client.cpp src/utils.cpp src/GameManager.cpp)

# Подключаем Google Test
enable_testing()
find_package(GTest REQUIRED)
add_executable(cp_test cp_test.cpp src/utils.cpp src/GameManager.cpp)
target_link_libraries(cp_test GTest::GTest GTest::Main)
add_test(NAME cp_test COMMAND cp_test)