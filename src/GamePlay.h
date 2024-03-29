//
// Created by nflet on 12/9/2022.
//
#pragma once

#include "Representation/Board.h"
#include "Representation/LookupLibrary.h"

Board *  initializeBoardFromFen(const char fen[],int fenLength,LookupLibrary* t, Board* board);
void simGame(LookupLibrary* t, const char fen[], int fenLength);
void playGame(LookupLibrary* t, Board* b, int sideFirst);