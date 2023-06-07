cmake_minimum_required(VERSION 3.22)
project(BoardSweep)

set(CMAKE_CXX_STANDARD 14)

add_executable(BoardSweep src/main.cpp src/Representation/Board.h src/Representation/Board.cpp src/GamePlay.h src/GamePlay.cpp src/BoardVisualization.h src/BoardVisualization.cpp src/Representation/Move.cpp src/Representation/Move.h src/Representation/UsefulConstants.h src/MoveGeneration/TargetGeneration.h src/MoveGeneration/TargetGeneration.cpp src/BitUtil.cpp src/BitUtil.h src/Representation/UsefulConstants.cpp src/MoveGeneration/MoveGeneration.h src/MoveGeneration/MoveGeneration.cpp src/Representation/LookupLibrary.h src/Representation/LookupLibrary.cpp src/Testing/Tester.cpp src/Testing/Tester.h src/Search/Eval.cpp src/Search/Eval.h src/Search/EvalConstants.cpp src/Search/EvalConstants.h src/Search/Search.cpp src/Search/Search.h)
