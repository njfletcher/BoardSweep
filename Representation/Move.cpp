//
// Created by nflet on 12/19/2022.
//

#include "Move.h"
#include <iostream>

Move::Move(int from, int to, bool isDoublePush, bool isCapture, bool isPromotion, bool isEnPassant, bool isCastle, int moved,
     int captured, int promoted){

    squareFrom = from;
    squareTo = to;
    doublePush = isDoublePush;
    capture = isCapture;
    promotion = isPromotion;
    enPassant = isEnPassant;
    castle = isCastle;
    movedPiece = moved;
    promotedTo = promoted;
    capturedPiece = captured;

}

void Move::toString(){
    std::cout <<squareFrom << " " << squareTo << " " << doublePush << capture << promotion << enPassant << castle << " " << movedPiece << " " << capturedPiece << std::endl;
}

