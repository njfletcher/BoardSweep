//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/LookupLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, LookupLibrary* t);
Move* generateAllMoves(Board* board, LookupLibrary* t, int* moveCount,Move* list,int depth);
void generateAllCaptures(Board* board, LookupLibrary* t, Move** moves, Move* list, int* moveCount,int depth);
std::vector<Move> findLegalMoves(Board* board, std::vector<Move> allMoves, LookupLibrary* t);
bool makeMove(Move m, Board* b,LookupLibrary* t,int depth);
void unmakeMove(Move m, Board* b,LookupLibrary* t,int depth);

unsigned long long Perft(int finishDepth, int printDepth, Board* board, LookupLibrary* t);
unsigned long long PerftWrapper(int finishDepth, int printDepth, Board* board, LookupLibrary* t);

