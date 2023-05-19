//
// Created by nflet on 5/18/2023.
//
#pragma once
#include "../Representation/TargetLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"

MovePair startAB(int currentDepth, TargetLibrary* t, Board* board, bool side);
MovePair searchAB(int currentDepth, int alpha, int beta, TargetLibrary* t, Board* board, bool side, Move m);


