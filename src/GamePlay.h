//
// Created by nflet on 12/9/2022.
//
#pragma once

#include "Representation/Board.h"
#include "Representation/TargetLibrary.h"

Board *  initializeBoardFromFen(const char fen[]);
void simGame(TargetLibrary* t, const char fen[]);