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

    unsigned int * bishopTargetCountLookups = initializeBishopTargetCountLookup(bishopTargetLookups);
    unsigned int * rookTargetCountLookups = initializeRookTargetCountLookup(rookTargetLookups);



    unsigned long long test = 0x101010101010101;

    //displayBitboard(test);

    cout<< (1ULL << 12) << endl;




    return 0;

}
