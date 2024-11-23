set(CMAKE_C_STANDARD 11)

add_executable(lab1
        main.cpp
        src/parent.cpp
        include/parent.h)

target_include_directories(lab1 PRIVATE include)

add_executable(child
        src/child.cpp
        include/parent.h)

target_include_directories(child PRIVATE include)

add_dependencies(lab1 child)
