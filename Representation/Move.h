//
// Created by nflet on 12/19/2022.
//
#pragma once


#include "UsefulConstants.h"

class Move{

public:
    Move(int from, int to, bool isDoublePush, bool isCapture, bool isPromotion, bool isEnPassant, bool isCastle, int moved,
               int captured, int promoted);

    int squareTo;
    int squareFrom;
    bool doublePush;
    bool capture;
    bool promotion;
    bool enPassant;
    bool castle;
    int movedPiece;
    int promotedTo;
    int capturedPiece;
    //enumColor turn;



};