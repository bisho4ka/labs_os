cmake_minimum_required(VERSION 3.10)
project(OS-labs)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование
enable_testing()

# Превращаем предупреждения о неинициализированных переменных в ошибки
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# Устанавливаем директории для выходных файлов
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Добавляем подпроекты
add_subdirectory(cp)
add_subdirectory(tests)