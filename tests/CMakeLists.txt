add_executable(lab5-7_test lab5-7_test.cpp ../lab5-7/src/tcontroller.cpp ../lab5-7/src/tmessaging.cpp ../lab5-7/src/tsearch.cpp ../lab5-7/src/ttopology.cpp ../lab5-7/src/tworker.cpp)

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
  TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)
target_link_libraries(lab5-7_test PRIVATE gtest_main zmq)

target_include_directories(lab5-7_test PRIVATE ../lab5-7/include)

add_test(lab5-7_test lab5-7_test)

include(GoogleTest)

gtest_discover_tests(lab5-7_test)