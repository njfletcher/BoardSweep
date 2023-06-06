#include <iostream>
#include "BoardVisualization.h"
#include "GamePlay.h"
#include "MoveGeneration/TargetGeneration.h"
#include "MoveGeneration/MoveGeneration.h"
#include "Representation/LookupLibrary.h"
#include "Testing/Tester.h"
#include "Search/Search.h"


using namespace std;
int main(int argc, char** argv) {

    LookupLibrary lookup;
    lookup.pawnSinglePushLookups = initializePawnSinglePushTargetLookups();
    lookup.pawnDoublePushLookups = initializePawnDoublePushTargetLookups();
    lookup.pawnAttackLookups = initializePawnAttackTargetLookups();
    lookup.knightMoveLookups = initializeKnightTargetLookups();
    lookup.kingMoveLookups = initializeKingTargetLookups();
    lookup.bishopTargetLookups = initializeBishopTargetLookups();
    lookup.rookTargetLookups = initializeRookTargetLookups();
    lookup.bishopMagicAttacks = initializeBishopMagicAttackTable(lookup.bishopTargetLookups);
    lookup.rookMagicAttacks = initializeRookMagicAttackTable(lookup.rookTargetLookups);
    initializeZobristArrays(&lookup);

    Board * board = initializeBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",&lookup);

    cout << argv[0] << endl;

    testAll(&lookup);
    //simGame(&lookup,"8/8/8/8/8/k7/p1K5/8 b - - 0 1");
    //simGame(&lookup,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //testPositionUnknown("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",6,&lookup);
    //board->currentDepth = 0;
    //cout << "nodes: " << PerftWrapper(6,400,board,&lookup) << endl;





    return 0;


}

