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

    //4th bit: black kingside, 3rd bit: black queenside, 2nd bit:white kingside, 1st bit: white queenside
    //set bit means right to castle
    unsigned int castleRights;

    //0-63 mean enPassant right exists on square, 64 means enPassant option does not exist
    unsigned int enPassSquare;

    unsigned int halfMoveCount;
    unsigned int fullMoveCount;

};
