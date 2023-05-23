//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/LookupLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, LookupLibrary* t);
std::vector<Move> generateAllMoves(Board* board, LookupLibrary* t);
void generateAllCaptures(Board* board, LookupLibrary* t, vector<Move>* moves);
std::vector<Move> findLegalMoves(Board* board, std::vector<Move> allMoves, LookupLibrary* t);
void makeMove(Move m, Board* b);
void unmakeMove(Move m, Board* b);

unsigned long long Perft(int finishDepth, int printDepth, Board* board, LookupLibrary* t);

void generateMovesCertainDepth(int depth,Board* board, LookupLibrary* t);