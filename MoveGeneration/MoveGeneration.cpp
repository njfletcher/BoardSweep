//
// Created by nflet on 1/5/2023.
//
#include "MoveGeneration.h"
#include "../BitUtil.h"
#include "../Representation/UsefulConstants.h"
#include "../Representation/TargetLibrary.h"
#include "TargetGeneration.h"
#include <iostream>
#include "../BoardVisualization.h"
using namespace std;

//white = 0 black =1;
unsigned long long getAttackMask(bool side,unsigned long long* bitboards, TargetLibrary* t){

    unsigned long long attack = 0ULL;

    unsigned long long whitePieces = bitboards[0];
    unsigned long long blackPieces = bitboards[1];
    unsigned long long allPieces = whitePieces | blackPieces;

   unsigned long long pawns = bitboards[2+side];
   while(pawns){

        int square = popLSB(&pawns);
        attack |= t->pawnAttackLookups[side][square];
    }

    unsigned long long knights = bitboards[4+side];
    while(knights){
        int square = popLSB(&knights);
        attack |= t->knightMoveLookups[square];
    }

    unsigned long long bishops = bitboards[6+side];
    while(bishops){
        int square = popLSB(&bishops);
        attack |= getBishopTargetFromBlockers(square,allPieces  & t->bishopTargetLookups[square],t->bishopMagicAttacks);
    }

    unsigned long long rooks = bitboards[8+side];
    while(rooks){
        int square = popLSB(&rooks);
        attack |= getRookTargetFromBlockers(square,allPieces & t->rookTargetLookups[square],t->rookMagicAttacks);
    }

    unsigned long long queens = bitboards[10+side];
    while(queens){
        int square = popLSB(&queens);
        unsigned long long blockers = (allPieces & t->rookTargetLookups[square]) | (allPieces & t->bishopTargetLookups[square]);
        attack |= getQueenTargetFromBlockers(square,blockers,t->rookMagicAttacks,t->bishopMagicAttacks);
    }

    unsigned long long kings = bitboards[12+side];
    while(kings){
        int square = popLSB(&kings);
        attack |= t->kingMoveLookups[square];
    }


    return attack;

}