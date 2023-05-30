//
// Created by nflet on 12/31/2022.
//
#pragma once

int countSetBits(unsigned long long bitboard);

int getIndexLSB(unsigned long long bitboard);

void popBit(unsigned long long* bitboardPtr, int index);

int popLSB(unsigned long long* bitboardPtr);

unsigned long long generateRandomBitboard();
unsigned long long generateSparseBitboard();