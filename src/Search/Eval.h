//
// Created by nflet on 5/16/2023.
//

#pragma once
#include "../Representation/Board.h"
#include "../Representation/LookupLibrary.h"


int evaluatePosition(Board* board, bool sideToMove,LookupLibrary* t,bool isCheckMate, bool isDraw, int depth);
