set(CMAKE_C_STANDARD 11)

add_library(library_1 SHARED src/library_1.cpp)
set_target_properties(library_1 PROPERTIES OUTPUT_NAME "library_1" PREFIX "")
target_include_directories(library_1 PRIVATE include)

add_library(library_2 SHARED src/library_2.cpp)
set_target_properties(library_2 PROPERTIES OUTPUT_NAME "library_2" PREFIX "")
target_include_directories(library_2 PRIVATE include)

add_executable(program_1 program_1.cpp)
add_executable(program_2 program_2.cpp)

target_include_directories(program_1 PRIVATE include)
target_include_directories(program_2 PRIVATE include)

target_link_libraries(program_1 PRIVATE library_1 dl)
target_link_libraries(program_2 PRIVATE library_2 dl)