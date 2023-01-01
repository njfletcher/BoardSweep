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
unsigned long long** initializePawnAttackLookups();

unsigned long long generateKnightTarget(int knightSquare);
unsigned long long* initializeKnightMoveLookups();

unsigned long long generateKingTarget(int kingSquare);
unsigned long long* initializeKingMoveLookups();

unsigned long long generateBishopTargetEmptyBoard(int bishopSquare);
unsigned long long generateRookTargetEmptyBoard(int rookSquare);
unsigned long long generateQueenTargetEmptyBoard(int queenSquare);

unsigned long long generateBishopTargetOnTheFly(int bishopSquare,unsigned long long allPieces);
unsigned long long generateRookTargetOnTheFly(int rookSquare,unsigned long long allPieces);
unsigned long long generateQueenTargetOnTheFly(int queenSquare,unsigned long long allPieces);

unsigned long long* initializeBishopTargetLookups();
unsigned long long* initializeRookTargetLookups();

unsigned long long* initializeBishopTargetCountLookup(unsigned long long * bishopMasks);
unsigned long long* initializeRookTargetCountLookup(unsigned long long * rookMasks);

