set(CMAKE_C_STANDARD 11)

add_executable(lab2 main.cpp src/filter.cpp include/filter.h)

target_include_directories(lab2 PRIVATE include)
target_link_libraries(lab2 PRIVATE Threads::Threads)
