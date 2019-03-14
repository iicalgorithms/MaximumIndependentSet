cmake_minimum_required(VERSION 2.3)
project(OMIS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pg")

set(SOURCE_FILES main.cpp Node.cpp Queue.cpp Vertex.cpp Graph.cpp)
set(COREMK_SOURCE_FILES CoreFromGraph.cpp Vertex.cpp Node.cpp)


add_executable(OMIS ${SOURCE_FILES})
add_executable(CoreMK ${COREMK_SOURCE_FILES})
