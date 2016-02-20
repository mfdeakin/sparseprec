
cmake_minimum_required (VERSION 3.3)
project(cdtprec)

file(GLOB TESTSRC "./test*.cpp")

add_executable(testCDT ${TESTSRC})

target_link_libraries(testCDT gtest pthread)

set_target_properties(testCDT PROPERTIES COMPILE_FLAGS "-std=c++11 -Wall")
