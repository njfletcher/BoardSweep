//
// Created by nflet on 12/19/2022.
//

#include "Move.h"

Move::Move(int from, int to, bool isCapture, bool isPromotion, bool isEnPassant, bool isCastle, int moved,
     int captured, int promoted){

    squareFrom = from;
    squareTo = to;
    capture = isCapture;
    promotion = isPromotion;
    enPassant = isEnPassant;
    castle = isCastle;
    movedPiece = moved;
    promotedTo = promoted;
    capturedPiece = captured;

}



