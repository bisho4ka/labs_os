cmake_minimum_required(VERSION 3.10)
project(cp)

set(CMAKE_CXX_STANDARD 17)

# Указываем исходные файлы
set(SOURCES
    src/client.cpp
    src/server.cpp
    main.cpp
)

# Создаем исполняемый файл
add_executable(cp ${SOURCES})

# Указываем пути к заголовочным файлам
target_include_directories(cp PRIVATE include)