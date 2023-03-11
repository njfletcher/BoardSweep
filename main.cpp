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

    Board * board = initializeBoardFromFen("2bb4/7p/3P3k/p1pNp2p/PpPKB2P/1P2P1P1/8/8 w - - 3 41");

    displayChessboard(board);

    vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    cout<< ms.size() << endl;

    vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);
    cout<< legals.size() << endl;

    /*for(Move m : ms){

        cout << "BEFORE=======================================" << endl;
        displayChessboard(board);

        cout << "MAKE____________________________________________________" << endl;
        makeMove(m,board);
        displayChessboard(board);

        cout << "UNMAKE+++++++++++++++++++++++++++++++++++++++++++++++++++" <<endl;
        unmakeMove(m,board);
        displayChessboard(board);
    }
     */





    return 0;

}

