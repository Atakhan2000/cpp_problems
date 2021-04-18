cmake_minimum_required(VERSION 3.19)

add_library(list list.h testList.cpp)
set_property(TARGET list PROPERTY CXX_STANDARD 17)

project(testList)

add_executable(testList
  testList.cpp
)