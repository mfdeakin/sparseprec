
cmake_minimum_required (VERSION 3.3)
project(cdtprec)

file(GLOB TESTSRC "./test*.cpp")

add_executable(testSparsePrec ${TESTSRC})

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE MATCHES "debug")
  #  message("Debug Flags Enabled")
  set_target_properties(testSparsePrec PROPERTIES COMPILE_FLAGS "-g -fsanitize=undefined -fsanitize=address")
  set_target_properties(testSparsePrec PROPERTIES LINK_FLAGS "-fsanitize=undefined -fsanitize=address")
endif(CMAKE_BUILD_TYPE MATCHES "debug")

target_link_libraries(testSparsePrec gtest pthread)

get_target_property(cflags testSparsePrec COMPILE_FLAGS)
set_target_properties(testSparsePrec PROPERTIES COMPILE_FLAGS "${cflags} -std=c++11 -Wall")
