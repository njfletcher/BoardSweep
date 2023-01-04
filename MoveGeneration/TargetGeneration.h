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

unsigned int* initializeBishopTargetCountLookup(unsigned long long * bishopMasks);
unsigned int* initializeRookTargetCountLookup(unsigned long long * rookMasks);

unsigned long long generateUniqueBlockerMask(int iteration, int changeableBits, unsigned long long attackMask);

void findMagicNumbers(unsigned long long* attacks, bool bishop);

unsigned long long** initializeRookMagicAttackTable(unsigned long long* rookAttacks);
unsigned long long** initializeBishopMagicAttackTable(unsigned long long* bishopAttacks);

unsigned long long getBishopTargetFromBlockers(int square, unsigned long long blockers, unsigned long long** magicAttacks);
unsigned long long getRookTargetFromBlockers(int square, unsigned long long blockers, unsigned long long** magicAttacks);