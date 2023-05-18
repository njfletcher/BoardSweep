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
Move::Move(){

    squareFrom = 0;
    squareTo = 0;
    doublePush = 0;
    capture = 0;
    promotion = 0;
    enPassant = 0;
    castle = 0;
    movedPiece = 0;
    promotedTo = 0;
    capturedPiece = 0;
}

void Move::toString(){
    std::cout <<squareFrom << " " << squareTo << " " << doublePush << capture << promotion << enPassant << castle << " " << movedPiece << " " << capturedPiece << " " << promotedTo << std::endl;
}
void Move::toUCI() {

    char cap = ' ';
    if(promotion){

        //subtract 2 from the index bc the bitboard piece index and piece char index are offset by two
        cap = PieceChars[promotedTo-2];
    }
    std::cout << Squares[squareFrom] << Squares[squareTo] << cap <<std::endl;
}

MovePair::MovePair(Move move, int eval) {

    m = move;
    evalScore = eval;

}

