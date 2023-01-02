//
// Created by nflet on 12/31/2022.
//


//Based on Brian Kernighan's algorithm
unsigned int countSetBits(unsigned long long bitboard){

    int count = 0;

    while(bitboard>0){

        bitboard = bitboard & (bitboard -1);
        count++;
    }

    return count;
}



//returns 0-63 if valid index, returns 64 if 0ULL
unsigned int getIndexLSB(unsigned long long bitboard){

    return countSetBits((bitboard & -bitboard) -1);

}

void popBit(unsigned long long* bitboardPtr, int index){

    unsigned long long indexBit = 1ULL <<index;

    if((*bitboardPtr) & indexBit) *bitboardPtr ^= (indexBit);

}
unsigned int popLSB(unsigned long long* bitboardPtr){

    unsigned int index = getIndexLSB(*bitboardPtr);

    popBit(bitboardPtr,index);

    return index;
}