//
// Created by nflet on 12/31/2022.
//
#pragma once


unsigned int countSetBits(unsigned long long bitboard);

unsigned int getIndexLSB(unsigned long long bitboard);

void popBit(unsigned long long* bitboardPtr, int index);

unsigned int popLSB(unsigned long long* bitboardPtr);

unsigned long long generateRandomBitboard(unsigned int* seed);
unsigned int xorShift(unsigned int* start);
unsigned long long generateSparseBitboard(unsigned int* seed);