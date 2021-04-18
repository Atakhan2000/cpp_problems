cmake_minimum_required(VERSION 3.19)

add_library(mm mm.h testMM.cpp)
set_property(TARGET mm PROPERTY CXX_STANDARD 17)

project(testMM)

add_executable(testMM
  testMM.cpp
)