#include <iostream>
#include "BoardVisualization.h"
#include "GamePlay.h"
#include "Representation/Move.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/TargetGeneration.h"
#include "Representation/TargetLibrary.h"
#include "Testing/Tester.h"
#include "Search/Search.h"

using namespace std;
int main(int argc, char** argv) {

    //hi
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

    Board * board = initializeBoardFromFen("8/5bk1/8/2Pp4/8/1K6/8/8 b - d6 0 1");

    cout << argv[0] << endl;

    //vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    //vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);
    //displayWholeBoard(board);


    cout << Perft(6,10,board,&lookup,board->sideToMove) << endl;




    //generateMovesCertainDepth(2,board,&lookup,board->sideToMove);

    //testAll(&lookup);
    //displayWholeBoard(board);
    simGame(&lookup);
    //int score = evaluatePosition(board,board->sideToMove,&lookup);
    //cout << score <<endl;

    //cout << board->sideToMove << endl;
   // MovePair pair = startAB(7,&lookup,board,board->sideToMove);
   // pair.m.toUCI();



    return 0;


}

