//
// Created by nflet on 1/5/2023.
//

#pragma once
#include "../Representation/TargetLibrary.h"
#include "../Representation/Board.h"

unsigned long long getAttackMask(bool side,unsigned long long* bitboards, TargetLibrary* t);
void generateAllQuietMoves(bool side,unsigned long long* bitboards, TargetLibrary* t);
void generateAllAttackMoves(bool side,Board* board, TargetLibrary* t);