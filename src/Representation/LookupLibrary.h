//
// Created by nflet on 1/6/2023.
//

#pragma once


class LookupLibrary {

public:

    unsigned long long ** pawnSinglePushLookups;
    unsigned long long ** pawnDoublePushLookups;
    unsigned long long ** pawnAttackLookups;
    //unsigned long long* wPawnAttacks;
    //unsigned long long* bPawnAttacks;

    unsigned long long * knightMoveLookups;

    unsigned long long * kingMoveLookups;

    unsigned long long * bishopTargetLookups;
    unsigned long long * rookTargetLookups;


    unsigned long long ** bishopMagicAttacks;
    unsigned long long ** rookMagicAttacks;

    //one for each square, for each piece type
    unsigned long long zobristPieces[12][64];

    //one for each possible 4 bit castle rights
    unsigned long long zobristCastles[16];
    unsigned long long zobristBlackTurn;

    //one for each file enpassant could occur on
    unsigned long long zobristEnPass[8];
};
