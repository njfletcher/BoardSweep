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
    lookup.pawnSinglePushLookups = initializePawnSinglePushLookups();
    lookup.pawnDoublePushLookups = initializePawnDoublePushLookups();
    lookup.pawnAttackLookups = initializePawnAttackLookups();
    lookup.knightMoveLookups = initializeKnightMoveLookups();
    lookup.kingMoveLookups = initializeKingMoveLookups();
    lookup.bishopTargetLookups = initializeBishopTargetLookups();
    lookup.rookTargetLookups = initializeRookTargetLookups();
    lookup.bishopMagicAttacks = initializeBishopMagicAttackTable(lookup.bishopTargetLookups);
    lookup.rookMagicAttacks = initializeRookMagicAttackTable(lookup.rookTargetLookups);



    Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    //displayBitboard(board->bitboards[0] | board->bitboards[1]);

    //generateAllQuietMoves(0,board->bitboards,&lookup);


    //displayBitboard(0x7C00000000000000);
    //displayBitboard(0x7C);


    //displayWholeBoard(board);
   /* vector<Move> ms = generateAllQuietMoves(board->sideToMove,board,&lookup);

    for(Move m : ms){

        cout << "BEFORE=======================================" << endl;
        displayBitboard(board->bitboards[0] | board->bitboards[1]);

        cout << "MAKE____________________________________________________" << endl;
        makeMove(m,board);
        displayBitboard(board->bitboards[0] | board->bitboards[1]);

        cout << "UNMAKE+++++++++++++++++++++++++++++++++++++++++++++++++++" <<endl;
        unmakeMove(m,board);
        displayBitboard(board->bitboards[0] | board->bitboards[1]);
    }*/
    generateAllMovesCertainDepth(10,board,&lookup,board->sideToMove);


    /*displayWholeBoard(board);
    Move m(34,26,0,0,0,0,0,3,0,0);
    makeMove(m,board);
    displayWholeBoard(board);
    unmakeMove(m,board);
    cout << " -----------------------" << endl;
    displayWholeBoard(board);
     */

    //vector<Move> moves = generateAllQuietMoves(board->sideToMove,board,&lookup);

    /*for(Move m: moves){

        displayWholeBoard(board);
        makeMove(m,board);
        displayWholeBoard(board);
        unmakeMove(m,board);
        displayWholeBoard(board);

    }
     */



        /* for(int i =0;i<64;i++){
             displayBitboard(lookup.pawnSinglePushLookups[0][i]);
         }
         */



    return 0;

}

