cmake_minimum_required(VERSION 3.19)

add_library(sort sort.h profile.h test_sort.cpp)
set_property(TARGET sort PROPERTY CXX_STANDARD 17)

project(test_sort)

add_executable(test_sort
  test_sort.cpp
)