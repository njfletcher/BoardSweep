#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"
#include "MoveGeneration/MoveGeneration.h"
#include "Representation/UsefulConstants.h"
#include "Representation/Move.h"
#include "Representation/TargetLibrary.h"
#include "Testing/Tester.h"
#include "BitUtil.h"
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;
int main(int argc, char** argv) {

    TargetLibrary lookup;
    lookup.pawnSinglePushLookups = initializePawnSinglePushTargetLookups();
    lookup.pawnDoublePushLookups = initializePawnDoublePushTargetLookups();
    lookup.pawnAttackLookups = initializePawnAttackTargetLookups();
    lookup.knightMoveLookups = initializeKnightTargetLookups();
    lookup.kingMoveLookups = initializeKingTargetLookups();
    lookup.bishopTargetLookups = initializeBishopTargetLookups();
    lookup.rookTargetLookups = initializeRookTargetLookups();
    lookup.bishopMagicAttacks = initializeBishopMagicAttackTable(lookup.bishopTargetLookups);
    lookup.rookMagicAttacks = initializeRookMagicAttackTable(lookup.rookTargetLookups);

    Board * board = initializeBoardFromFen("8/5bk1/8/2Pp4/8/1K6/8/8 w - d6 0 1");

    cout << argv[0] << endl;

    //vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    //vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);
    //displayWholeBoard(board);


    //cout << Perft(6,10,board,&lookup,board->sideToMove) << endl;




    //generateMovesCertainDepth(2,board,&lookup,board->sideToMove);

    testAll(&lookup);



    return 0;


}

