//
// Created by nflet on 12/31/2022.
//

#include "BitUtil.h"
#include <stdlib.h>


//Based on Brian Kernighan's algorithm
int countSetBits(unsigned long long bitboard){

    int count = 0;

    while(bitboard>0){

        bitboard = bitboard & (bitboard -1);
        count++;
    }

    return count;
}



//returns 0-63 given a valid set bitboard
//assumes check for bitboard == 0ULL is done elsewhere
int getIndexLSB(unsigned long long bitboard){

    return countSetBits((bitboard & -bitboard) -1);

}

//assumes index given is valid and is set.
void popBit(unsigned long long* bitboardPtr, int index){

    unsigned long long indexBit = 1ULL <<index;

    *bitboardPtr &= ~(indexBit);

}

//pops lsb, returns the index
int popLSB(unsigned long long* bitboardPtr){

    unsigned int index = getIndexLSB(*bitboardPtr);

    //popBit(bitboardPtr,index);
    *bitboardPtr = *bitboardPtr & (*bitboardPtr -1);

    return index;
}

unsigned long long generateRandomBitboard(){

    unsigned long long first = ((unsigned long long) rand()) & 0xFFFF;
    unsigned long long second = ((unsigned long long) rand()) & 0xFFFF;
    unsigned long long third = ((unsigned long long) rand()) & 0xFFFF;
    unsigned long long fourth = ((unsigned long long) rand()) & 0xFFFF;


    return first | (second << 16) | (third << 32) | (fourth << 48);

}


//Based on Tord Romstad's method
unsigned long long generateSparseBitboard(){

    return generateRandomBitboard() & generateRandomBitboard() & generateRandomBitboard();
}




