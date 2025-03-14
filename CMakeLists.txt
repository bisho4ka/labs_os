cmake_minimum_required(VERSION 3.10)
project(Battleship)

set(CMAKE_CXX_STANDARD 17)

# Сервер
add_executable(server server/server.cpp server/game.cpp)

# Клиент
add_executable(client client/client.cpp)

# Тесты
find_package(GTest REQUIRED)
add_executable(test_server tests/test_server.cpp server/game.cpp)
target_link_libraries(test_server GTest::GTest GTest::Main)