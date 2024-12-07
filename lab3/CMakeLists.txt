set(CMAKE_C_STANDARD 11)

add_executable(lab3
        main.cpp
        src/parent.cpp
        include/parent.h)

target_include_directories(lab3 PRIVATE include)
target_link_libraries(lab3 rt pthread)

add_executable(child
        src/child.cpp
        include/parent.h)

target_include_directories(child PRIVATE include)
target_link_libraries(child rt pthread)

add_dependencies(lab3 child)
