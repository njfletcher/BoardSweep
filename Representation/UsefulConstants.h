//
// Created by nflet on 12/19/2022.
//
#pragma once

//capital letter = white, lowercase = black
enum enumPiece
{
    allWhite = 0,
    allBlack = 1,
    P = 2,
    p = 3,
    N = 4,
    n = 5,
    B = 6,
    b = 7,
    R = 8,
    r = 9,
    Q = 10,
    q = 11,
    K = 12,
    k = 13
};

enum enumColor{
    white,
    black
};
extern const char PieceChars[];

extern const char* Squares[];
extern const unsigned long long FileMasks[];

extern const unsigned long long RankMasks[];

extern const int BishopTargetCount[];
extern const int RookTargetCount[];

extern const unsigned long long BishopMagics[];
extern const unsigned long long RookMagics[];

extern const unsigned long long CastleSquares[];