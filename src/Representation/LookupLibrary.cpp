//
// Created by nflet on 5/28/2023.
//
#include "LookupLibrary.h"


LookupLibrary::~LookupLibrary(){

    for(int i =0; i<2;i++){
        delete[] pawnSinglePushLookups[i];
        delete[] pawnAttackLookups[i];
        delete[] pawnDoublePushLookups[i];
    }
    delete[] knightMoveLookups;
    delete[] kingMoveLookups;
    delete[] bishopTargetLookups;
    delete[] rookTargetLookups;

    for(int i =0; i <64; i++){
        delete[] bishopMagicAttacks[i];
        delete[] rookMagicAttacks[i];
    }

}