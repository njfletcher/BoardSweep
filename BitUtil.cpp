//
// Created by nflet on 12/31/2022.
//


//Based on Brian Kernighan's algorithm
int countSetBits(unsigned long long bitboard){

    int count = 0;

    while(bitboard>0){

        bitboard = bitboard & (bitboard -1);
        count++;
    }

    return count;
}



//returns 0-63 if valid index, returns 64 if 0ULL
int getIndexLSB(unsigned long long bitboard){

    return countSetBits((bitboard & -bitboard) -1);

}