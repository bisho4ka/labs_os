# CMake generated Testfile for 
# Source directory: /workspaces/labs_os-1/tests
# Build directory: /workspaces/labs_os-1/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
include("/workspaces/labs_os-1/tests/lab5-7_test[1]_include.cmake")
add_test(lab5-7_test "lab5-7_test")
set_tests_properties(lab5-7_test PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/labs_os-1/tests/CMakeLists.txt;16;add_test;/workspaces/labs_os-1/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
