#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"
#include "MoveGeneration/MoveGeneration.h"
#include "Representation/UsefulConstants.h"
#include "Representation/Move.h"
#include "Representation/TargetLibrary.h"
#include "BitUtil.h"
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;
int main() {

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

    Board * board = initializeBoardFromFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");

    vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);

    displayWholeBoard(board);
    for(Move m : ms){

        m.toString();
        /*cout << "BEFORE=======================================" << endl;
        displayWholeBoard(board);

        cout << "MAKE____________________________________________________" << endl;
        makeMove(board->sideToMove,m,board);
        displayWholeBoard(board);

        cout << "UNMAKE+++++++++++++++++++++++++++++++++++++++++++++++++++" <<endl;
        unmakeMove(board->sideToMove,m,board);
        displayWholeBoard(board);
         */


    }





    cout << Perft(1,board,&lookup,board->sideToMove) << endl;
    //generateMovesCertainDepth(2,board,&lookup,board->sideToMove);



    return 0;

}

