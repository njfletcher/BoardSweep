cmake_minimum_required(VERSION 3.22)
project(BoardSweep)

set(CMAKE_CXX_STANDARD 14)

add_executable(BoardSweep main.cpp Representation/Board.h Representation/Board.cpp GameStart.h GameStart.cpp BoardVisualization.h BoardVisualization.cpp Representation/Move.cpp Representation/Move.h Representation/PieceTypeAndColor.h MoveGeneration/TargetGeneration.h MoveGeneration/TargetGeneration.cpp BitUtil.cpp BitUtil.h)
