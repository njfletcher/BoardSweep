//
// Created by nflet on 12/19/2022.
//
#pragma once

//capital letter = white, lowercase = black
enum enumPiece
{
    allWhite = 0,
    allBlack = 1,
    P = 'P',
    p = 'p',
    N = 'N',
    n = 'n',
    B = 'B',
    b = 'b',
    R = 'R',
    r = 'r',
    Q = 'Q',
    q = 'q',
    K = 'K',
    k = 'k'
};


enum enumColor{
    white,
    black
};

extern const unsigned long long FileMasks[];

extern const unsigned long long RankMasks[];

extern const int BishopTargetCount[];
extern const int RookTargetCount[];

extern const unsigned long long BishopMagics[];
extern const unsigned long long RookMagics[];