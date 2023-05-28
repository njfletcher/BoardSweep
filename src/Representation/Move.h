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
    Move* nextMove;
    Move* prevMove;
    //enumColor turn;

};

void addToMoveList(Move** start, Move*move);

class MovePair{

public:
    MovePair(Move move, int eval);
    Move m;
    int evalScore;
};