//
// Created by nflet on 5/18/2023.
//
#pragma once
#include "../Representation/LookupLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"

MovePair startAB(int currentDepth, LookupLibrary* t, Board* board);
MovePair searchAB(int currentDepth, int alpha, int beta, LookupLibrary* t, Board* board,Move m);
void initializeZobristArrays(LookupLibrary* t);



