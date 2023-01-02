//
// Created by nflet on 12/7/2022.
//
#pragma once
#include "UsefulConstants.h"




class Board{

    //using little endian mapping


public:

    unsigned long long bitboards[14];
    bool sideToMove;

    //4th bit: white kingside, 3rd bit: white queenside, 2nd bit:black kingside, 1st bit: black queenside
    //set bit means right to castle
    unsigned int castleRights;

    //0-63 mean enPassant right exists on square, 64 means enPassant option does not exist
    unsigned int enPassSquare;

    unsigned int halfMoveCount;
    unsigned int fullMoveCount;

    //using single char enum items so fen string chars can be used to directly index bitboard array
    //Uppercase = white piece, lowercase = black piece

    unsigned long long getBitboardByType(enumPiece piece) const;
    unsigned long long getBitboardByTypeAndColor(enumPiece piece, enumColor c) const;





};
