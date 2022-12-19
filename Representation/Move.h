//
// Created by nflet on 12/19/2022.
//

#ifndef BOARDSWEEP_MOVE_H
#define BOARDSWEEP_MOVE_H

#endif //BOARDSWEEP_MOVE_H

#include "Board.h"

class Move{

public:


protected:
    int squareTo;
    int squareFrom;
    Board * current;
    bool capture;
    bool promotion;
    bool enPassant;
    bool castle;
    enumPiece movedPiece;
    enumPiece promotedTo;
    enumPiece capturedPiece;
    enumColor turn;
};