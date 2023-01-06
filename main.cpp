#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"
#include "MoveGeneration/MoveGeneration.h"
#include "Representation/UsefulConstants.h"
#include "Representation/TargetLibrary.h"
#include "BitUtil.h"
#include <iomanip>

using namespace std;
int main() {

    TargetLibrary lookup;

    lookup.pawnAttackLookups = initializePawnAttackLookups();
    lookup.knightMoveLookups = initializeKnightMoveLookups();
    lookup.kingMoveLookups = initializeKingMoveLookups();
    lookup.bishopTargetLookups = initializeBishopTargetLookups();
    lookup.rookTargetLookups = initializeRookTargetLookups();
    lookup.bishopMagicAttacks = initializeBishopMagicAttackTable(lookup.bishopTargetLookups);
    lookup.rookMagicAttacks = initializeRookMagicAttackTable(lookup.rookTargetLookups);



    Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    displayBitboard(board->bitboards[1]);


    displayBitboard(getAttackMask(true,board->bitboards,&lookup) & ~board->bitboards[1]);


    return 0;

}

