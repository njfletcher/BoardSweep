//
// Created by nflet on 1/5/2023.
//
#include "MoveGeneration.h"
#include "../BitUtil.h"
#include "../Representation/UsefulConstants.h"
#include "../Representation/TargetLibrary.h"
#include "TargetGeneration.h"
#include <iostream>
#include "../BoardVisualization.h"
using namespace std;

//white = 0 black =1;
unsigned long long getAttackMask(bool side,unsigned long long* bitboards, TargetLibrary* t){

    unsigned long long attack = 0ULL;

    unsigned long long whitePieces = bitboards[0];
    unsigned long long blackPieces = bitboards[1];
    unsigned long long allPieces = whitePieces | blackPieces;

   unsigned long long pawns = bitboards[P+side];
   while(pawns){

        int square = popLSB(&pawns);
        attack |= t->pawnAttackLookups[side][square];
    }

    unsigned long long knights = bitboards[N+side];
    while(knights){
        int square = popLSB(&knights);
        attack |= t->knightMoveLookups[square];
    }

    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int square = popLSB(&bishops);
        attack |= getBishopTargetFromBlockers(square,allPieces  & t->bishopTargetLookups[square],t->bishopMagicAttacks);
    }

    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int square = popLSB(&rooks);
        attack |= getRookTargetFromBlockers(square,allPieces & t->rookTargetLookups[square],t->rookMagicAttacks);
    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int square = popLSB(&queens);

        attack |= getQueenTargetFromBlockers(square,allPieces,t);
    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int square = popLSB(&king);
    attack |= t->kingMoveLookups[square];



    return attack;

}


void generateAllQuietMoves(bool side,Board* board, TargetLibrary* t){

    unsigned long long* bitboards = board->bitboards;

    unsigned long long allPieces = bitboards[0] | bitboards[1];

    unsigned long long pawns = bitboards[P+side];

    while(pawns){
        int fromSquare = popLSB(&pawns);

        unsigned long long pawnSingle = t->pawnSinglePushLookups[side][fromSquare];
        unsigned long long pawnDouble = t->pawnDoublePushLookups[side][fromSquare];


        unsigned long long pawnMoveFirst = (pawnSingle & ~allPieces);
        unsigned long long pawnMoveSecond = (pawnDouble & ~allPieces);
        unsigned long long pawnAllMoves = pawnMoveFirst | ((pawnMoveFirst && pawnMoveSecond) * pawnMoveSecond);

        while(pawnAllMoves){
            int toSquare = popLSB(&pawnAllMoves);

            cout<< "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;

        }

    }
    //for enpassant update in future, if double move, set enpassant square to square of single push target

    unsigned long long knights = bitboards[N+side];
    while(knights){
        int fromSquare = popLSB(&knights);
        unsigned long long knightAllMoves = t->knightMoveLookups[fromSquare] & ~allPieces;
        while(knightAllMoves){
            int toSquare = popLSB(&knightAllMoves);

            cout<< "from: " << fromSquare << "to: " << toSquare << "knight" << " side: " << side << endl;
        }
    }


    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int fromSquare = popLSB(&bishops);
        unsigned long long bishopAllMoves =  getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks) & ~allPieces;

        while(bishopAllMoves){
            int toSquare = popLSB(&bishopAllMoves);
            cout<< "from: " << fromSquare << "to: " << toSquare << "bishop" << " side: " << side << endl;
        }
    }

    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int fromSquare = popLSB(&rooks);
        unsigned long long rookAllMoves =  getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks) & ~allPieces;

        while(rookAllMoves){
            int toSquare = popLSB(&rookAllMoves);
            cout<< "from: " << fromSquare << "to: " << toSquare << "rook" << " side: " << side << endl;
        }
    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int fromSquare = popLSB(&queens);
        unsigned long long queenAllMoves = getQueenTargetFromBlockers(fromSquare,allPieces,t) & ~allPieces;

        while(queenAllMoves){
            int toSquare = popLSB(&queenAllMoves);
            cout<< "from: " << fromSquare << "to: " << toSquare << "queen" << " side: " << side << endl;
        }
    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int kingSquare = popLSB(&king);
    unsigned long long kingAllMoves =  t->kingMoveLookups[kingSquare] & ~allPieces;
    while(kingAllMoves){
        int toSquare = popLSB(&kingAllMoves);
        cout<< "from: " << kingSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }


    bool sideHasQueenSideCastle = board->castleRights & (1ULL<<(0+(side * 2))) && 1;
    bool sideHasKingSideCastle = board->castleRights & (1ULL<<(1+(side * 2))) && 1;

    //kingside rook before castle is always 3 bits left of king,
    int kingRookSquare = kingSquare +3;

    //queenside rook before castle is always 4 bits right of king
    int queenRookSquare = kingSquare -4;

    if(sideHasKingSideCastle){

        //shift king left 2 and shift kingSideRook right 2

        cout<< "from: " << kingSquare << "to: " << kingSquare+2 << "kingCastle" << " side: " << side << endl;

    }
    if(sideHasQueenSideCastle){

        //shift king right 3 and shift queenSideRook left 3
        cout<< "from: " << kingSquare << "to: " << kingSquare-3 << "kingCastle" << " side: " << side << endl;
    }




}

void generateAllAttackMoves(bool side,Board* board, TargetLibrary* t) {

    unsigned long long* bitboards = board->bitboards;

    unsigned long long enemyPieces = bitboards[!side];
    unsigned long long friendlyPieces = bitboards[side];

    unsigned long long allPieces = enemyPieces | friendlyPieces;


    unsigned long long pawns = bitboards[P + side];

    while (pawns) {
        int fromSquare = popLSB(&pawns);

        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;

        unsigned long long enPassBit = 1ULL << board->enPassSquare;
        unsigned long long enPassCap = pawnAttackTargets & enPassBit;

        if(enPassCap){
            cout << "from: " << fromSquare << "to: " << board->enPassSquare << "pawnEnPass" << " side: " << side << endl;
        }

        while (pawnNormalCaptures) {
            int toSquare = popLSB(&pawnNormalCaptures);

            cout << "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;

        }

    }

    unsigned long long knights = bitboards[N + side];
    while (knights) {
        int fromSquare = popLSB(&knights);
        unsigned long long knightAllCaptures = t->knightMoveLookups[fromSquare] & enemyPieces;
        while (knightAllCaptures) {
            int toSquare = popLSB(&knightAllCaptures);

            cout << "from: " << fromSquare << "to: " << toSquare << "knight" << " side: " << side << endl;
        }
    }


    unsigned long long bishops = bitboards[B + side];
    while (bishops) {
        int fromSquare = popLSB(&bishops);
        unsigned long long bishopAllCaptures =
                getBishopTargetFromBlockers(fromSquare, allPieces & t->bishopTargetLookups[fromSquare],
                                            t->bishopMagicAttacks) & enemyPieces;
        while (bishopAllCaptures) {
            int toSquare = popLSB(&bishopAllCaptures);
            cout << "from: " << fromSquare << "to: " << toSquare << "bishop" << " side: " << side << endl;
        }
    }

    unsigned long long rooks = bitboards[R + side];
    while (rooks) {
        int fromSquare = popLSB(&rooks);
        unsigned long long rookAllCaptures =
                getRookTargetFromBlockers(fromSquare, allPieces & t->rookTargetLookups[fromSquare],
                                          t->rookMagicAttacks) & enemyPieces;
        while (rookAllCaptures) {
            int toSquare = popLSB(&rookAllCaptures);
            cout << "from: " << fromSquare << "to: " << toSquare << "rook" << " side: " << side << endl;
        }
    }

    unsigned long long queens = bitboards[Q + side];
    while (queens) {
        int fromSquare = popLSB(&queens);
        unsigned long long queenAllCaptures = getQueenTargetFromBlockers(fromSquare, allPieces, t) & enemyPieces;

        while (queenAllCaptures) {
            int toSquare = popLSB(&queenAllCaptures);
            cout << "from: " << fromSquare << "to: " << toSquare << "queen" << " side: " << side << endl;
        }
    }


    unsigned long long king = bitboards[K + side];

    //assumption is that there will always have to be a single king on the board for each side
    int fromSquare = popLSB(&king);
    unsigned long long kingAllCaptures = t->kingMoveLookups[fromSquare] & enemyPieces;
    while (kingAllCaptures) {
        int toSquare = popLSB(&kingAllCaptures);
        cout << "from: " << fromSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }



}