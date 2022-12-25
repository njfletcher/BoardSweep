//
// Created by nflet on 12/25/2022.
//
#include "MoveGeneration.h"
#include "../BoardVisualization.h"

//to get other ranks from first rank, shift left by increments of 8.
//rank i = firstRank << (8 * (i-1))
unsigned long long firstRank = (unsigned long long)0xFF;


//to get other files from first file(a), shift left by increments of 1.
//file i = aFile <<(i-1)
unsigned long long aFile = (unsigned long long)0x101010101010101;


unsigned long long generateWhitePawnSingleTarget(unsigned long long whitePawns, unsigned long long emptySquares){

    unsigned long long targetSquares = whitePawns << 8;

    return targetSquares & emptySquares;
}

unsigned long long generateBlackPawnSingleTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long targetSquares = blackPawns >> 8;

    return targetSquares & emptySquares;
}

unsigned long long generateWhitePawnDoubleTarget(unsigned long long whitePawns, unsigned long long emptySquares){

    unsigned long long fourthRank = firstRank << 24;
    unsigned long long singlePushTargets = generateWhitePawnSingleTarget(whitePawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets << 8;
    return doubleTargets & emptySquares & fourthRank;

}

unsigned long long generateBlackPawnDoubleTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long fifthRank = firstRank << 32;
    unsigned long long singlePushTargets = generateBlackPawnSingleTarget(blackPawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets >> 8;
    return doubleTargets & emptySquares & fifthRank;
}