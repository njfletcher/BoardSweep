//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/TargetLibrary.h"
#include "../Representation/Board.h"
#include "../Representation/Move.h"
#include <vector>

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, TargetLibrary* t);
std::vector<Move> generateAllQuietMoves(bool side,Board* board, TargetLibrary* t);
std::vector<Move> generateAllAttackMoves(bool side,Board* board, TargetLibrary* t);
void makeMove(Move m, Board* b);
void unmakeMove(Move m, Board* b);

void generateAllMovesCertainDepth(int depth,Board* board, TargetLibrary* t);