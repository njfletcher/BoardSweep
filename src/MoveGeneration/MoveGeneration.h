//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/LookupLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, LookupLibrary* t);
std::vector<Move> generateAllMoves(bool side,Board* board, LookupLibrary* t);
void generateAllCaptures(bool side, Board* board, LookupLibrary* t, vector<Move>* moves);
std::vector<Move> findLegalMoves(bool side, Board* board, std::vector<Move> allMoves, LookupLibrary* t);
void makeMove(bool side,Move m, Board* b);
void unmakeMove(bool side,Move m, Board* b);

unsigned long long Perft(int finishDepth, int printDepth, Board* board, LookupLibrary* t,bool side);

void generateMovesCertainDepth(int depth,Board* board, LookupLibrary* t,bool side);