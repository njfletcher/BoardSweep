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

    Board * board = initializeBoardFromFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");

    vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);


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





    //cout << Perft(1,board,&lookup,board->sideToMove) << endl;
    //generateMovesCertainDepth(2,board,&lookup,board->sideToMove);




    return 0;

}

