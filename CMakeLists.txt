cmake_minimum_required(VERSION 3.22)
project(BoardSweep)

set(CMAKE_CXX_STANDARD 14)

add_executable(BoardSweep main.cpp Representation/Board.h Representation/Board.cpp GameStart.h GameStart.cpp BoardVisualization.h BoardVisualization.cpp Representation/Move.cpp Representation/Move.h Representation/UsefulConstants.h MoveGeneration/TargetGeneration.h MoveGeneration/TargetGeneration.cpp BitUtil.cpp BitUtil.h Representation/UsefulConstants.cpp MoveGeneration/MoveGeneration.h MoveGeneration/MoveGeneration.cpp Representation/TargetLibrary.h Testing/Tester.cpp Testing/Tester.h Search/Eval.cpp Search/Eval.h Search/EvalConstants.cpp Search/EvalConstants.h)
