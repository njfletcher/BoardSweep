//
// Created by nflet on 12/19/2022.
//
#pragma once

#include "Board.h"

class Move{

public:
    int squareTo;
    int squareFrom;
    //Board * current;
    bool capture;
    bool promotion;
    bool enPassant;
    bool castle;
    enumPiece movedPiece;
    enumPiece promotedTo;
    enumPiece capturedPiece;
    //enumColor turn;



};