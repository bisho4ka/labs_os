cmake_minimum_required(VERSION 3.10)
project(lab5-7)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск ZeroMQ
find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

# Поиск потоков (pthread)
find_package(Threads REQUIRED)

# Добавление исполняемых файлов
add_executable(lab5-7_controller
    main_controller.cpp
    src/tcontroller.cpp
    src/tmessaging.cpp
    src/ttopology.cpp
)

add_executable(lab5-7_worker
    main_worker.cpp
    src/tworker.cpp
    src/tmessaging.cpp
    src/ttopology.cpp
)

# Подключение библиотек
target_link_libraries(lab5-7_controller PRIVATE ${ZMQ_LIBRARIES} Threads::Threads)
target_link_libraries(lab5-7_worker PRIVATE ${ZMQ_LIBRARIES} Threads::Threads)

# Подключение заголовочных файлов
target_include_directories(lab5-7_controller PRIVATE include)
target_include_directories(lab5-7_worker PRIVATE include)