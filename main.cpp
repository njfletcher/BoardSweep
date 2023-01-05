#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"
#include "Representation/UsefulConstants.h"
#include "BitUtil.h"
#include <iomanip>

using namespace std;
int main() {

    //Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    //displayWholeBoard(board);

    unsigned long long ** pawnAttackLookups = initializePawnAttackLookups();

    unsigned long long* wPawnAttacks = *pawnAttackLookups;
    unsigned long long* bPawnAttacks = *(++pawnAttackLookups);

    unsigned long long * knightMoveLookups = initializeKnightMoveLookups();

    unsigned long long * kingMoveLookups = initializeKingMoveLookups();

    unsigned long long * bishopTargetLookups = initializeBishopTargetLookups();
    unsigned long long * rookTargetLookups = initializeRookTargetLookups();


    unsigned long long ** bishopMagicAttacks = initializeBishopMagicAttackTable(bishopTargetLookups);
    unsigned long long ** rookMagicAttacks = initializeRookMagicAttackTable(rookTargetLookups);



    int square = 27;
    unsigned long long attackMask = bishopTargetLookups[square];
    int bitCount = BishopTargetCount[square];
    unsigned int maxVal = 1<< bitCount;



   for(int currVal =0; currVal < maxVal; currVal++){

        unsigned long long uniqueBlockerMask = generateUniqueBlockerMask(currVal,bitCount,attackMask);
        // use magic number to hash and get index then put into the array

        //int index = (int)((uniqueBlockerMask * RookMagics[square]) >> (64 - bitCount));


       displayBitboard(uniqueBlockerMask);

       unsigned long long result = getBishopTargetFromBlockers(square,uniqueBlockerMask,bishopMagicAttacks);

       displayBitboard(result);

       cout << "+++++++++++++++++++++++++++++++++++++" << endl;


    }










    return 0;

}
