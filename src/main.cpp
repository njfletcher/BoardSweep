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

    //hi
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

    //vector<Move> ms = generateAllMoves(board->sideToMove,board,&lookup);

    //vector<Move> legals = findLegalMoves(board->sideToMove,board,ms,&lookup);
    //displayWholeBoard(board);


    //cout << Perft(6,10,board,&lookup,board->sideToMove) << endl;

    /*
    cout << lookup.zobristBlackTurn << endl;

    unsigned long long* castles = lookup.zobristCastles;
    for(int i =0;i<16;i++){

        cout << *castles << endl;
        castles++;
    }

    unsigned long long* enPasses = lookup.zobristEnPass;
    for(int i =0; i<8;i++){
        cout << *enPasses << endl;
        enPasses++;
    }

    unsigned long long* pieces = *lookup.zobristPieces;
    for(int i =0; i<12 * 64;i++){
        cout << *pieces << endl;
        pieces++;
    }

    */
    //generateMovesCertainDepth(2,board,&lookup,board->sideToMove);

    //testPositionUnknown("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",5,&lookup);
    //testAll(&lookup);
    //displayWholeBoard(board);
    //simGame(&lookup,"8/8/8/8/8/k7/p1K5/8 b - - 0 1");
    //int score = evaluatePosition(board,board->sideToMove,&lookup);
    //cout << score <<endl;

    //cout << board->sideToMove << endl;
   // MovePair pair = startAB(7,&lookup,board,board->sideToMove);
   // pair.m.toUCI();
   board->currentDepth = 0;
    cout << "nodes: " << PerftWrapper(4,400,board,&lookup) << endl;
    //displayBitboard(0x70);
    //displayBitboard(0x1E);


    return 0;


}

