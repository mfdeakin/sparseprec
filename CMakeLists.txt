
cmake_minimum_required (VERSION 3.3)
project(cdtprec)

file(GLOB TESTSRC "./test*.cpp")

add_executable(testSparsePrec ${TESTSRC})

target_link_libraries(testSparsePrec gtest pthread)

set_target_properties(testSparsePrec PROPERTIES COMPILE_FLAGS "-std=c++11 -Wall")
