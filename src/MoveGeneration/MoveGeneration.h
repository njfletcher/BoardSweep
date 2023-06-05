//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/LookupLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,const unsigned long long* bitboards, LookupLibrary* t);
unsigned long long* generateAllMoves(Board* board, LookupLibrary* t, int* moveCount,unsigned long long* list);
bool checkIfSquareAttacked(bool side, Board* board, LookupLibrary* t, int square);
void generateAllCaptures(Board* board, LookupLibrary* t, Move** moves, Move* list, int* moveCount);
bool makeMove(unsigned long long m, Board* b,LookupLibrary* t, bool isInSearch);
void unmakeMove(unsigned long long m, Board* b,LookupLibrary* t);

unsigned long long Perft(int finishDepth, int printDepth, Board* board, LookupLibrary* t);
unsigned long long PerftWrapper(int finishDepth, int printDepth, Board* board, LookupLibrary* t);

