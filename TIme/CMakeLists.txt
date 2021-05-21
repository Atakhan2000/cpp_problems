cmake_minimum_required(VERSION 3.10)

add_library(time sort.h mm.h hash.h profile.h algs_time.cpp)
set_property(TARGET time PROPERTY CXX_STANDARD 17)

project(not_mtime)

add_executable(not_mtime
  algs_time.cpp
)