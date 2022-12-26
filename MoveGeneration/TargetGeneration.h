//
// Created by nflet on 12/25/2022.
//

#pragma once

unsigned long long generateWPawnSinglePushTarget(unsigned long long whitePawns, unsigned long long allPieces);

unsigned long long generateBPawnSinglePushTarget(unsigned long long blackPawns, unsigned long long allPieces);

unsigned long long generateWPawnDoublePushTarget(unsigned long long whitePawns, unsigned long long allPieces);

unsigned long long generateBPawnDoublePushTarget(unsigned long long blackPawns, unsigned long long allPieces);


unsigned long long generateWPawnEastAttackTarget(unsigned long long whitePawns);

unsigned long long generateWPawnWestAttackTarget(unsigned long long whitePawns);

unsigned long long generateBPawnEastAttackTarget(unsigned long long blackPawns);

unsigned long long generateBPawnWestAttackTarget(unsigned long long blackPawns);

unsigned long long** initializePawnMoveLookups();

unsigned long long** initializePawnAttackLookups();

unsigned long long generateKnightTarget(unsigned long long knight);

unsigned long long* initializeKnightMoveLookups();

unsigned long long generateKingTarget(unsigned long long king);

unsigned long long* initializeKingMoveLookups();