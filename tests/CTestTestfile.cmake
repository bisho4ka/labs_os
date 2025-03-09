# CMake generated Testfile for 
# Source directory: /workspaces/labs_os-1/tests
# Build directory: /workspaces/labs_os-1/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(lab4_test_1 "lab4_test_1")
set_tests_properties(lab4_test_1 PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/labs_os-1/tests/CMakeLists.txt;25;add_test;/workspaces/labs_os-1/tests/CMakeLists.txt;0;")
add_test(lab4_test_2 "lab4_test_2")
set_tests_properties(lab4_test_2 PROPERTIES  _BACKTRACE_TRIPLES "/workspaces/labs_os-1/tests/CMakeLists.txt;26;add_test;/workspaces/labs_os-1/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
