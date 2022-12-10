cmake_minimum_required(VERSION 3.22)
project(BoardSweep)

set(CMAKE_CXX_STANDARD 14)

add_executable(BoardSweep main.cpp BoardRepresentation.h BoardRepresentation.cpp GameStart.h GameStart.cpp BoardVisualization.h BoardVisualization.cpp)
