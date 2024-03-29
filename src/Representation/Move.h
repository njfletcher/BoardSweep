//
// Created by nflet on 12/19/2022.
//
#pragma once


#include "UsefulConstants.h"

class Move{

public:
    Move(int from, int to, bool isDoublePush, bool isCapture, bool isPromotion, bool isEnPassant, bool isCastle, int moved,
               int captured, int promoted);

    Move();

    void toString();
    void toUCI();

    bool isValid;
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


class MovePair{

public:
    MovePair(unsigned long long move, int eval);
    unsigned long long m;
    int evalScore;
};

unsigned long long packageMove(unsigned int from, unsigned int to, bool doubleP, bool cap, bool pro, bool enP, bool cas, unsigned int moved, unsigned int captured, unsigned int promoted);