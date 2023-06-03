//
// Created by nflet on 12/19/2022.
//

#include "Move.h"
#include <iostream>

Move::Move(int from, int to, bool isDoublePush, bool isCapture, bool isPromotion, bool isEnPassant, bool isCastle, int moved,
     int captured, int promoted){

    isValid = true;
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

    isValid = false;
    squareFrom = 0;
    squareTo = 0;
    doublePush = false;
    capture = false;
    promotion = false;
    enPassant = false;
    castle = false;
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



MovePair::MovePair(unsigned long long move, int eval) {

    m = move;
    evalScore = eval;

}

unsigned long long packageMove(unsigned int from, unsigned int to, bool doubleP, bool cap, bool pro, bool enP, bool cas, unsigned int moved, unsigned int captured, unsigned int promoted){

    unsigned long long move = 0;

    move |= from;
    move |= to << 6;
    unsigned long long flags = doubleP | (cap <<1) | (pro << 2) | (enP << 3) | (cas << 4);
    move |= flags << 12;
    unsigned long long pieces = moved | (captured << 4) | (promoted << 8);
    move |= pieces << 17;
    return move;
}