cmake_minimum_required(VERSION 3.3)
project(Interpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES run.cpp Interpreter.cpp Parser.cpp UserFunction.cpp)
add_executable(Interpreter ${SOURCE_FILES})