cmake_minimum_required(VERSION 3.10)
project(DistributedSystem)

find_package(ZeroMQ REQUIRED)

add_executable(manager manager.cpp)
target_link_libraries(manager zmq)

add_executable(node node.cpp)
target_link_libraries(node zmq)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(test_manager test_manager.cpp)
target_link_libraries(test_manager ${GTEST_LIBRARIES} pthread zmq)