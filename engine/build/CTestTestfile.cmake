# CMake generated Testfile for 
# Source directory: D:/Programmering/chess-cpp
# Build directory: D:/Programmering/chess-cpp/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[Testing]=] "D:/Programmering/chess-cpp/build/Debug/runTests.exe")
  set_tests_properties([=[Testing]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/Programmering/chess-cpp/CMakeLists.txt;68;add_test;D:/Programmering/chess-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[Testing]=] "D:/Programmering/chess-cpp/build/Release/runTests.exe")
  set_tests_properties([=[Testing]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/Programmering/chess-cpp/CMakeLists.txt;68;add_test;D:/Programmering/chess-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[Testing]=] "D:/Programmering/chess-cpp/build/MinSizeRel/runTests.exe")
  set_tests_properties([=[Testing]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/Programmering/chess-cpp/CMakeLists.txt;68;add_test;D:/Programmering/chess-cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[Testing]=] "D:/Programmering/chess-cpp/build/RelWithDebInfo/runTests.exe")
  set_tests_properties([=[Testing]=] PROPERTIES  _BACKTRACE_TRIPLES "D:/Programmering/chess-cpp/CMakeLists.txt;68;add_test;D:/Programmering/chess-cpp/CMakeLists.txt;0;")
else()
  add_test([=[Testing]=] NOT_AVAILABLE)
endif()
subdirs("_deps/googletest-build")
