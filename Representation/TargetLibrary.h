//
// Created by nflet on 1/6/2023.
//

#pragma once


class TargetLibrary {

public:

    unsigned long long ** pawnAttackLookups;
    //unsigned long long* wPawnAttacks;
    //unsigned long long* bPawnAttacks;

    unsigned long long * knightMoveLookups;

    unsigned long long * kingMoveLookups;

    unsigned long long * bishopTargetLookups;
    unsigned long long * rookTargetLookups;


    unsigned long long ** bishopMagicAttacks;
    unsigned long long ** rookMagicAttacks;
};
