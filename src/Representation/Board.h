//
// Created by nflet on 12/7/2022.
//
#pragma once
#include "UsefulConstants.h"
#include <vector>

using namespace std;

const int maxSearchDepth = 20;

class Board{

    //using little endian mapping

public:

    //first two are side bitboards, rest are individual piece bitboards.
    unsigned long long bitboards[14];

    //0 is white, 1 is black
    bool sideToMove;

    //4th bit: black kingside, 3rd bit: black queenside, 2nd bit:white kingside, 1st bit: white queenside
    //set bit means right to castle
    //vector<unsigned int> castleRights;
    unsigned int castleRights[maxSearchDepth];

    //0-63 mean enPassant right exists on square, 64 means enPassant option does not exist.
    //vector<unsigned int> enPassSquares;
    unsigned int enPassSquares[maxSearchDepth];

    //when this reaches 100, a draw by the 50 rule move can happen
    //count gets reset to 0 when there is a pawn move or a capture from any piece
    //vector<unsigned int> fiftyMoveRuleHalfMoves;
    unsigned int fiftyMoveRuleHalfMoves[maxSearchDepth];

    unsigned int fullMoveCount;

    /*"triple occurrence of position.[1] Two positions are by definition "the same" if the same types of pieces occupy the same squares,
     * the same player has the move, the remaining castling rights are the same and the possibility to capture en passant is the same"*/
    //move history, using zobrist keys
    vector<unsigned long long> moveHistory;

    //zobrist key for current position
    unsigned long long currentPosition;

    //current depth of search
    int currentDepth;

};
