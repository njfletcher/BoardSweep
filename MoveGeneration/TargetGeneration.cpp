//
// Created by nflet on 12/25/2022.
//
#include "TargetGeneration.h"
#include "../BoardVisualization.h"

//to get other ranks from first rank, shift left by increments of 8.
//rank i = firstRank << (8 * (i-1))
unsigned long long firstRank = (unsigned long long)0xFF;


//to get other files from first file(a), shift left by increments of 1.
//file i = aFile <<(i-1)
unsigned long long aFile = (unsigned long long)0x101010101010101;


unsigned long long generateWPawnSinglePushTarget(unsigned long long whitePawns, unsigned long long emptySquares){

    unsigned long long targetSquares = whitePawns << 8;

    return targetSquares & emptySquares;
}

unsigned long long generateBPawnSinglePushTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long targetSquares = blackPawns >> 8;

    return targetSquares & emptySquares;
}

unsigned long long generateWPawnDoublePushTarget(unsigned long long whitePawns, unsigned long long emptySquares){

    unsigned long long fourthRank = firstRank << 24;
    unsigned long long singlePushTargets = generateWPawnSinglePushTarget(whitePawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets << 8;
    return doubleTargets & emptySquares & fourthRank;

}

unsigned long long generateBPawnDoublePushTarget(unsigned long long blackPawns, unsigned long long emptySquares){

    unsigned long long fifthRank = firstRank << 32;
    unsigned long long singlePushTargets = generateBPawnSinglePushTarget(blackPawns,emptySquares);
    unsigned long long doubleTargets = singlePushTargets >> 8;
    return doubleTargets & emptySquares & fifthRank;
}

unsigned long long generateWPawnEastAttackTarget(unsigned long long whitePawns){
    return (whitePawns << 9) & (~aFile);
}

unsigned long long generateWPawnWestAttackTarget(unsigned long long whitePawns){
    return (whitePawns << 7) & (~(aFile<<7));
}

unsigned long long generateBPawnEastAttackTarget(unsigned long long blackPawns){
    return (blackPawns >> 7) & (~aFile);
}

unsigned long long generateBPawnWestAttackTarget(unsigned long long blackPawns){
    return (blackPawns >> 9) & (~(aFile<<7));
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

    //in order 1-8
    knightTargets |= (knight<<17) & (~aFile);
    knightTargets |= (knight << 15) & (~(aFile<<7));
    knightTargets |= (knight << 6) & (~(aFile<<7 | (aFile <<6)));
    knightTargets |= (knight >> 10) & (~(aFile<<7 | (aFile <<6)));
    knightTargets |= (knight >> 17) & (~(aFile<<7));
    knightTargets |= (knight >> 15) & (~aFile);
    knightTargets |= (knight >> 6) & (~(aFile | (aFile << 1)));
    knightTargets |= (knight << 10) & (~(aFile | (aFile << 1)));

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

    unsigned long long king = 1ULL << kingSquare;
    unsigned long long kingTargets =0;

    //in order 1-8

    kingTargets |= (king << 7) & (~(aFile<<7));
    kingTargets |= (king >> 1) & (~(aFile<<7));
    kingTargets |= (king >> 9) & (~(aFile<<7));
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


    unsigned long long hFile = aFile<<7;
    unsigned long long eightRank = firstRank<<56;
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
    unsigned long long eightRank = firstRank<<56;
    unsigned long long hFile = aFile<<7;
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


    unsigned long long hFile = aFile<<7;
    unsigned long long bishopTargets =0;
    unsigned long long bishopNE = (1ULL << bishopSquare) & (~hFile);

    //northeast ray
    while(bishopNE){
        unsigned long long next = bishopNE <<9;
        bishopTargets |= next;
        bishopNE = next & ~hFile & ~allPieces;

    }

    unsigned long long bishopNW = (1ULL << bishopSquare) & (~aFile);

    //northwest ray
    while(bishopNW){
        unsigned long long next = bishopNW <<7;
        bishopTargets |= next;
        bishopNW = next & ~aFile & ~allPieces;

    }

    unsigned long long bishopSE = (1ULL << bishopSquare) & (~hFile);

    //southeast ray
    while(bishopSE){
        unsigned long long next = bishopSE >>7;
        bishopTargets |= next;
        bishopSE = next & ~hFile & ~allPieces;

    }

    unsigned long long bishopSW = (1ULL << bishopSquare) & (~aFile);

    //southwest ray
    while(bishopSW){
        unsigned long long next = bishopSW >>9;
        bishopTargets |= next;
        bishopSW = next & ~aFile & ~allPieces;

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
    unsigned long long eightRank = firstRank<<56;
    unsigned long long hFile = aFile<<7;
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