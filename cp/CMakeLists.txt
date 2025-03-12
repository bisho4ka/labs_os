cmake_minimum_required(VERSION 3.10)
project(cp)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск потоков (pthread)
find_package(Threads REQUIRED)

# Добавление исполняемых файлов
add_executable(cp_main
    main.cpp
    src/server.cpp
    src/client.cpp
    src/game.cpp
)

# Подключение библиотек
target_link_libraries(cp_main PRIVATE Threads::Threads)

# Подключение заголовочных файлов
target_include_directories(cp_main PRIVATE include)