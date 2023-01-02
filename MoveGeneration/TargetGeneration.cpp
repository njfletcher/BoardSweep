//
// Created by nflet on 12/25/2022.
//
#include <iostream>
#include "TargetGeneration.h"
#include "../BoardVisualization.h"
#include "../BitUtil.h"
#include "../Representation/UsefulConstants.h"

using namespace std;


unsigned long long generateWPawnSinglePushTarget(unsigned long long whitePawns, unsigned long long emptySquares){

    unsigned long long targetSquares = whitePawns << 8;

    return targetSquares & emptySquares;
}

unsigned long long generateBPawnSinglePushTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long targetSquares = blackPawns >> 8;

    return targetSquares & emptySquares;
}

unsigned long long generateWPawnDoublePushTarget(unsigned long long whitePawns, unsigned long long emptySquares){


    unsigned long long singlePushTargets = generateWPawnSinglePushTarget(whitePawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets << 8;
    return doubleTargets & emptySquares & RankMasks[3];

}

unsigned long long generateBPawnDoublePushTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long singlePushTargets = generateBPawnSinglePushTarget(blackPawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets >> 8;
    return doubleTargets & emptySquares & RankMasks[4];
}

unsigned long long generateWPawnEastAttackTarget(unsigned long long whitePawns){
    return (whitePawns << 9) & (~FileMasks[0]);
}

unsigned long long generateWPawnWestAttackTarget(unsigned long long whitePawns){
    return (whitePawns << 7) & (~FileMasks[7]);
}

unsigned long long generateBPawnEastAttackTarget(unsigned long long blackPawns){
    return (blackPawns >> 7) & (~FileMasks[0]);
}

unsigned long long generateBPawnWestAttackTarget(unsigned long long blackPawns){
    return (blackPawns >> 9) & (~FileMasks[7]);
}

unsigned long long** initializePawnAttackLookups(){

    unsigned long long** lookups = new unsigned long long*[2];

    //white pawns, one mask for each square
    lookups[0] = new unsigned long long[64];
    //black pawns, one mask for each square
    lookups[1] = new unsigned long long[64];

    for(int i = 0; i <64; i++){

        unsigned long long setBit = 1ULL << i;

        lookups[0][i] = generateWPawnEastAttackTarget(setBit) | generateWPawnWestAttackTarget(setBit);
        lookups[1][i] = generateBPawnEastAttackTarget(setBit) | generateBPawnWestAttackTarget(setBit);
    }



    return lookups;
}

unsigned long long generateKnightTarget(int knightSquare){
    /*
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 2 0 1 0 0 0
    0 3 0 0 0 8 0 0
    0 0 0 N 0 0 0 0
    0 4 0 0 0 7 0 0
    0 0 5 0 6 0 0 0
     */

    unsigned long long knight = 1ULL << knightSquare;
    unsigned long long knightTargets =0;

    unsigned long long aFile = FileMasks[0];
    unsigned long long bFile = FileMasks[1];
    unsigned long long gFile = FileMasks[6];
    unsigned long long hFile = FileMasks[7];


    //in order 1-8
    knightTargets |= (knight<<17) & (~aFile);
    knightTargets |= (knight << 15) & (~hFile);
    knightTargets |= (knight << 6) & (~(hFile | gFile));
    knightTargets |= (knight >> 10) & (~(hFile | gFile));
    knightTargets |= (knight >> 17) & (~hFile);
    knightTargets |= (knight >> 15) & (~aFile);
    knightTargets |= (knight >> 6) & (~(aFile | bFile));
    knightTargets |= (knight << 10) & (~(aFile | bFile));

    return knightTargets;


}

unsigned long long* initializeKnightMoveLookups(){

    unsigned long long* lookups = new unsigned long long[64];

    for(int i = 0; i <64; i++){
        lookups[i] = generateKnightTarget(i);
    }

    return lookups;

}

unsigned long long generateKingTarget(int kingSquare){
    /*
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 1 8 7 0 0 0
    0 0 2 K 6 0 0 0
    0 0 3 4 5 0 0 0
    0 0 0 0 0 0 0 0
     */

    unsigned long long aFile = FileMasks[0];
    unsigned long long hFile = FileMasks[7];

    unsigned long long king = 1ULL << kingSquare;
    unsigned long long kingTargets =0;

    //in order 1-8

    kingTargets |= (king << 7) & (~hFile);
    kingTargets |= (king >> 1) & (~hFile);
    kingTargets |= (king >> 9) & (~hFile);
    kingTargets |= (king >> 8);
    kingTargets |= (king >> 7) & (~aFile);
    kingTargets |= (king << 1) & (~aFile);
    kingTargets |= (king << 9) & (~aFile);
    kingTargets |= (king << 8);

    return kingTargets;

}

unsigned long long* initializeKingMoveLookups(){

    unsigned long long* lookups = new unsigned long long[64];

    for(int i = 0; i <64; i++){
        lookups[i] = generateKingTarget(i);
    }

    return lookups;
}

//used in magic bitboard hashing process.
//note: target bits at edge of each ray(on border) do not matter for magic bitboard process
//so they are not included in the returned map
unsigned long long generateBishopTargetEmptyBoard(int bishopSquare){

    /*
   0 0 0 0 0 0 0 0
   0 0 0 0 0 0 0 1
   1 0 0 0 0 0 1 0
   0 1 0 0 0 1 0 0
   0 0 1 0 1 0 0 0
   0 0 0 B 0 0 0 0
   0 0 1 0 1 0 0 0
   0 1 0 0 0 1 0 0
    */


    unsigned long long hFile = FileMasks[7];
    unsigned long long eightRank = RankMasks[7];
    unsigned long long firstRank = RankMasks[0];
    unsigned long long aFile = FileMasks[0];
    unsigned long long bishopTargets =0;
    unsigned long long bishopNE = (1ULL << bishopSquare) & (~hFile) & (~eightRank);

    //northeast ray
    while(bishopNE){
        unsigned long long next = bishopNE <<9 & ~hFile & ~eightRank;
        bishopTargets |= next;
        bishopNE = next;

    }

    unsigned long long bishopNW = (1ULL << bishopSquare) & (~aFile) & (~eightRank);

    //northwest ray
    while(bishopNW){
        unsigned long long next = bishopNW <<7  & ~aFile & (~eightRank);
        bishopTargets |= next;
        bishopNW = next;

    }

    unsigned long long bishopSE = (1ULL << bishopSquare) & (~hFile) & (~firstRank);

    //southeast ray
    while(bishopSE){
        unsigned long long next = bishopSE >>7 & ~hFile & (~firstRank);
        bishopTargets |= next;
        bishopSE = next;

    }

    unsigned long long bishopSW = (1ULL << bishopSquare) & (~aFile) & (~firstRank);

    //southwest ray
    while(bishopSW){
        unsigned long long next = bishopSW >>9  & ~aFile & (~firstRank);
        bishopTargets |= next;
        bishopSW = next;

    }

    return bishopTargets;

}

//used in magic bitboard hashing process.
//note: target bits at edge of each ray(on border) do not matter for magic bitboard process
//so they are not included in the returned map
unsigned long long generateRookTargetEmptyBoard(int rookSquare){
    /*
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      1 1 1 R 1 1 1 1
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
       */
    unsigned long long hFile = FileMasks[7];
    unsigned long long eightRank = RankMasks[7];
    unsigned long long firstRank = RankMasks[0];
    unsigned long long aFile = FileMasks[0];
    unsigned long long rookTargets =0;
    unsigned long long rookN = (1ULL << rookSquare) & (~eightRank);

    //north ray
    while(rookN){
        unsigned long long next = rookN <<8  & ~eightRank;
        rookTargets |= next;
        rookN = next;

    }

    unsigned long long rookE = (1ULL << rookSquare) & (~hFile);

    //east ray
    while(rookE){
        unsigned long long next = rookE <<1  & ~hFile;
        rookTargets |= next;
        rookE = next;

    }

    unsigned long long rookS = (1ULL << rookSquare) & (~firstRank);

    //south ray
    while(rookS){
        unsigned long long next = rookS >>8 & ~firstRank;
        rookTargets |= next;
        rookS = next;

    }

    unsigned long long rookW = (1ULL << rookSquare) & (~aFile);

    //west ray
    while(rookW){
        unsigned long long next = rookW >>1 & ~aFile;
        rookTargets |= next;
        rookW = next;

    }

    return rookTargets;
}

unsigned long long generateQueenTargetEmptyBoard(int queenSquare){

    return generateBishopTargetEmptyBoard(queenSquare) | generateRookTargetEmptyBoard(queenSquare);
}

unsigned long long generateBishopTargetOnTheFly(int bishopSquare, unsigned long long allPieces){
    /*
  0 0 0 0 0 0 0 0
  0 0 0 0 0 0 0 1
  1 0 0 0 0 0 1 0
  0 1 0 0 0 1 0 0
  0 0 1 0 1 0 0 0
  0 0 0 B 0 0 0 0
  0 0 1 0 1 0 0 0
  0 1 0 0 0 1 0 0
   */


    unsigned long long hFile = FileMasks[7];
    unsigned long long eightRank = RankMasks[7];
    unsigned long long firstRank = RankMasks[0];
    unsigned long long aFile = FileMasks[0];
    unsigned long long bishopTargets =0;
    unsigned long long bishopNE = (1ULL << bishopSquare) & (~hFile)  & (~eightRank);

    //northeast ray
    while(bishopNE){
        unsigned long long next = bishopNE <<9;
        bishopTargets |= next;
        bishopNE = next & ~hFile & ~allPieces  & (~eightRank);

    }

    unsigned long long bishopNW = (1ULL << bishopSquare) & (~aFile)  & (~eightRank);

    //northwest ray
    while(bishopNW){
        unsigned long long next = bishopNW <<7;
        bishopTargets |= next;
        bishopNW = next & ~aFile & ~allPieces  & (~eightRank);

    }

    unsigned long long bishopSE = (1ULL << bishopSquare) & (~hFile) & (~firstRank);

    //southeast ray
    while(bishopSE){
        unsigned long long next = bishopSE >>7;
        bishopTargets |= next;
        bishopSE = next & ~hFile & ~allPieces & (~firstRank);

    }

    unsigned long long bishopSW = (1ULL << bishopSquare) & (~aFile) & (~firstRank);

    //southwest ray
    while(bishopSW){
        unsigned long long next = bishopSW >>9;
        bishopTargets |= next;
        bishopSW = next & ~aFile & ~allPieces & (~firstRank);

    }

    return bishopTargets;
}

unsigned long long generateRookTargetOnTheFly(int rookSquare,unsigned long long allPieces){
/*
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
      1 1 1 R 1 1 1 1
      0 0 0 1 0 0 0 0
      0 0 0 1 0 0 0 0
       */
    unsigned long long hFile = FileMasks[7];
    unsigned long long eightRank = RankMasks[7];
    unsigned long long firstRank = RankMasks[0];
    unsigned long long aFile = FileMasks[0];
    unsigned long long rookTargets =0;
    unsigned long long rookN = (1ULL << rookSquare) & (~eightRank);

    //north ray
    while(rookN){
        unsigned long long next = rookN <<8;
        rookTargets |= next;
        rookN = next & ~eightRank & ~allPieces;

    }

    unsigned long long rookE = (1ULL << rookSquare) & (~hFile);

    //east ray
    while(rookE){
        unsigned long long next = rookE <<1;
        rookTargets |= next;
        rookE = next & ~hFile & ~allPieces;

    }

    unsigned long long rookS = (1ULL << rookSquare) & (~firstRank);

    //south ray
    while(rookS){
        unsigned long long next = rookS >>8;
        rookTargets |= next;
        rookS = next & ~firstRank & ~allPieces;

    }

    unsigned long long rookW = (1ULL << rookSquare) & (~aFile);

    //west ray
    while(rookW){
        unsigned long long next = rookW >>1;
        rookTargets |= next;
        rookW = next & ~aFile & ~allPieces;

    }

    return rookTargets;
}

unsigned long long generateQueenTargetOnTheFly(int queenSquare,unsigned long long allPieces){

    return generateBishopTargetOnTheFly(queenSquare,allPieces) | generateRookTargetOnTheFly(queenSquare,allPieces);
}

unsigned long long* initializeBishopTargetLookups(){

    unsigned long long* lookups = new unsigned long long[64];

    for(int i = 0; i <64; i++){
        lookups[i] = generateBishopTargetEmptyBoard(i);
    }

    return lookups;

}
unsigned long long* initializeRookTargetLookups(){

    unsigned long long* lookups = new unsigned long long[64];

    for(int i = 0; i <64; i++){
        lookups[i] = generateRookTargetEmptyBoard(i);
    }

    return lookups;
}
unsigned int* initializeBishopTargetCountLookup(unsigned long long * bishopMasks){

    unsigned int* lookups = new unsigned int[64];

    for(int i = 0; i <64; i++){
        lookups[i] = countSetBits(bishopMasks[i]);
    }

    return lookups;

}
unsigned int* initializeRookTargetCountLookup(unsigned long long * rookMasks){
    unsigned int* lookups = new unsigned int[64];

    for(int i = 0; i <64; i++){
        lookups[i] = countSetBits(rookMasks[i]);
    }

    return lookups;

}

//based on algorithm found in Fritz Reul's publication
unsigned long long generateUniqueBlockerMask(int iteration, int changeableBits, unsigned long long attackMask){

    unsigned long long uniqueMask = 0ULL;

    unsigned long long mask = attackMask;

    //loop over bits in attack mask one by one
    for (int count = 0; count < changeableBits; count++)
    {

        int square = getIndexLSB(mask);

        popBit(&mask, square);

        //add popped bit to uniqueMask if the iteration number and the shift share bits
        if (iteration & (1 << count)){

            uniqueMask |= (1ULL << square);
        }
    }

    return uniqueMask;

}

unsigned long long** initializeRookMagicAttackTable(unsigned long long* bitCounts, unsigned long long* rookAttacks){


    unsigned long long** lookups = new unsigned long long*[64];

    for(int square = 0; square <64; square++){

        lookups[square] = new unsigned long long[4096];

        unsigned long long attackMask = rookAttacks[square];
        int bitCount = bitCounts[square];
        unsigned long long upperBound = 1ULL << bitCount;

        for(int iteration =0; iteration < upperBound; iteration++){

            unsigned long long uniqueBlockerMask = generateUniqueBlockerMask(iteration,bitCount,attackMask);
            // use magic number to hash and get index then put into the array
        }


    }

    return lookups;
}

unsigned long long** initializeBishopMagicAttackTable(unsigned long long* bitCounts, unsigned long long* bishopAttacks){


    unsigned long long** lookups = new unsigned long long*[64];

    for(int square = 0; square <64; square++){

        lookups[square] = new unsigned long long[512];

        unsigned long long attackMask = bishopAttacks[square];
        int bitCount = bitCounts[square];
        unsigned long long upperBound = 1ULL << bitCount;

        for(int iteration =0; iteration < upperBound; iteration++){

            unsigned long long uniqueBlockerMask = generateUniqueBlockerMask(iteration,bitCount,attackMask);
            // use magic number to hash and get index then put into the array
        }


    }

    return lookups;
}