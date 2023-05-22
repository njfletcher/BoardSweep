//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/TargetLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, TargetLibrary* t);
std::vector<Move> generateAllMoves(bool side,Board* board, TargetLibrary* t);
void generateAllCaptures(bool side, Board* board, TargetLibrary* t, vector<Move>* moves);
std::vector<Move> findLegalMoves(bool side, Board* board, std::vector<Move> allMoves, TargetLibrary* t);
void makeMove(bool side,Move m, Board* b);
void unmakeMove(bool side,Move m, Board* b);

unsigned long long Perft(int finishDepth, int printDepth, Board* board, TargetLibrary* t,bool side);

void generateMovesCertainDepth(int depth,Board* board, TargetLibrary* t,bool side);