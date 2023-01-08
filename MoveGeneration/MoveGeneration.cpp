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

    unsigned long long kings = bitboards[K+side];
    while(kings){
        int square = popLSB(&kings);
        attack |= t->kingMoveLookups[square];
    }


    return attack;

}


void generateAllQuietMoves(bool side,unsigned long long* bitboards, TargetLibrary* t){


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

    unsigned long long kings = bitboards[K+side];
    while(kings){
        int fromSquare = popLSB(&kings);
        unsigned long long kingAllMoves =  t->kingMoveLookups[fromSquare] & ~allPieces;
        while(kingAllMoves){
            int toSquare = popLSB(&kingAllMoves);
            cout<< "from: " << fromSquare << "to: " << toSquare << "king" << " side: " << side << endl;
        }
    }


}