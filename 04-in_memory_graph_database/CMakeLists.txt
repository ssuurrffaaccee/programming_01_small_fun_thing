cmake_minimum_required(VERSION 3.5)
project(dagoba_project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(dagoba  STATIC
 src/runnable_op/runnable_op.cpp 
 src/dagoba.cpp
)
target_include_directories(dagoba  PUBLIC
include)

enable_testing()
add_executable(mytest test/test.cpp)
target_link_libraries(mytest dagoba)