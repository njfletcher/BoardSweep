/*
 * Created by Nicholas Fletcher on 12/31/2022.
 * BitUtil: all the methods needed for more complicated bit
 * operations needed throughout the engine.
*/

#include "BitUtil.h"
#include <stdlib.h>

const int bitLocation[64] = {
        0, 47,  1, 56, 48, 27,  2, 60,
        57, 49, 41, 37, 28, 16,  3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11,  4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30,  9, 24,
        13, 18,  8, 12,  7,  6,  5, 63
};

//Based on Brian Kernighan's algorithm
unsigned int countSetBits(unsigned long long bitboard){

    unsigned int count = 0;

    while(bitboard>0){

        bitboard = bitboard & (bitboard -1);
        count++;
    }

    return count;
}


/*getIndexLSB
 * Uses De Bruijn bitscan attributed to Martin Läuter. Takes a
 * U64 bitboard and returns the index of the least significant bit.
 * This expects the passed in bitboard to not be 0.
 * Move generation got a nearly x3 speedup when using this over
 * methods such as ones that use Brian Kernighan's countBits.
 */
unsigned int getIndexLSB(unsigned long long bitboard){

    //return countSetBits((bitboard & -bitboard) -1);
    const unsigned long long debruijnNum = 0x03f79d71b4cb0a89;
    int index = ((bitboard ^ (bitboard-1)) * debruijnNum) >> 58;
    return bitLocation[index];


}

//assumes index given is valid and is set.
void popBit(unsigned long long* bitboardPtr, int index){

    unsigned long long indexBit = 1ULL <<index;

    *bitboardPtr &= ~(indexBit);

}

//pops lsb, returns the index
unsigned int popLSB(unsigned long long* bitboardPtr){

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




