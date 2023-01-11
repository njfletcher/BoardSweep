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
#include <vector>
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


vector<Move> generateAllQuietMoves(bool side,Board* board, TargetLibrary* t){

    vector<Move> moveList;

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

            //check for any promotions
            unsigned long long relevantRank = RankMasks[7+(-7*side)];

            if((1ULL<<toSquare) & relevantRank){
                Move m(fromSquare,toSquare,0,1,0,0,P+side,0,N+side);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m1(fromSquare,toSquare,0,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m2(fromSquare,toSquare,0,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m3(fromSquare,toSquare,0,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;

            }
            else{
                Move m(fromSquare,toSquare,0,0,0,0,P+side,0,0);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;
            }


        }

    }


    unsigned long long knights = bitboards[N+side];
    while(knights){
        int fromSquare = popLSB(&knights);
        unsigned long long knightAllMoves = t->knightMoveLookups[fromSquare] & ~allPieces;
        while(knightAllMoves){

            int toSquare = popLSB(&knightAllMoves);

            Move m(fromSquare,toSquare,0,0,0,0,N+side,0,0);
            moveList.push_back(m);
            cout<< "from: " << fromSquare << "to: " << toSquare << "knight" << " side: " << side << endl;
        }
    }


    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int fromSquare = popLSB(&bishops);
        unsigned long long bishopAllMoves =  getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks) & ~allPieces;

        while(bishopAllMoves){

            int toSquare = popLSB(&bishopAllMoves);
            Move m(fromSquare,toSquare,0,0,0,0,B+side,0,0);
            moveList.push_back(m);
            cout<< "from: " << fromSquare << "to: " << toSquare << "bishop" << " side: " << side << endl;
        }
    }

    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int fromSquare = popLSB(&rooks);
        unsigned long long rookAllMoves =  getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks) & ~allPieces;

        while(rookAllMoves){
            int toSquare = popLSB(&rookAllMoves);

            Move m(fromSquare,toSquare,0,0,0,0,R+side,0,0);
            moveList.push_back(m);
            cout<< "from: " << fromSquare << "to: " << toSquare << "rook" << " side: " << side << endl;
        }
    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int fromSquare = popLSB(&queens);
        unsigned long long queenAllMoves = getQueenTargetFromBlockers(fromSquare,allPieces,t) & ~allPieces;

        while(queenAllMoves){

            int toSquare = popLSB(&queenAllMoves);
            Move m(fromSquare,toSquare,0,0,0,0,Q+side,0,0);
            moveList.push_back(m);
            cout<< "from: " << fromSquare << "to: " << toSquare << "queen" << " side: " << side << endl;
        }
    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int kingSquare = popLSB(&king);
    unsigned long long kingAllMoves =  t->kingMoveLookups[kingSquare] & ~allPieces;
    while(kingAllMoves){

        int toSquare = popLSB(&kingAllMoves);
        Move m(kingSquare,toSquare,0,0,0,0,K+side,0,0);
        moveList.push_back(m);
        cout<< "from: " << kingSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }


    bool sideHasQueenSideCastle = board->castleRights & (1ULL<<(0+(side * 2))) && 1;
    bool sideHasKingSideCastle = board->castleRights & (1ULL<<(1+(side * 2))) && 1;

    if(sideHasKingSideCastle){

        //shift king left 2 and shift kingSideRook right 2

        Move m(kingSquare,kingSquare+2,0,0,0,1,K+side,0,0);
        moveList.push_back(m);
        cout<< "from: " << kingSquare << "to: " << kingSquare+2 << "kingCastle" << " side: " << side << endl;

    }
    if(sideHasQueenSideCastle){

        //shift king right 2 and shift queenSideRook left 3

        Move m(kingSquare,kingSquare-2,0,0,0,1,K+side,0,0);
        moveList.push_back(m);
        cout<< "from: " << kingSquare << "to: " << kingSquare-3 << "kingCastle" << " side: " << side << endl;
    }


    return moveList;

}

vector<Move> generateAllAttackMoves(bool side,Board* board, TargetLibrary* t) {

    vector<Move> moveList;

    unsigned long long* bitboards = board->bitboards;

    unsigned long long enemyPieces = bitboards[!side];
    unsigned long long friendlyPieces = bitboards[side];

    unsigned long long allPieces = enemyPieces | friendlyPieces;


    unsigned long long pawns = bitboards[P + side];

    while (pawns) {
        int fromSquare = popLSB(&pawns);

        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;


        while (pawnNormalCaptures) {
            int toSquare = popLSB(&pawnNormalCaptures);
            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            //check for any promotions
            unsigned long long relevantRank = RankMasks[7+(-7*side)];

            if((1ULL<<toSquare) & relevantRank){
                Move m(fromSquare,toSquare,1,1,0,0,P+side,0,N+side);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m1(fromSquare,toSquare,1,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m2(fromSquare,toSquare,1,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m3(fromSquare,toSquare,1,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;

            }
            else{
                Move m(fromSquare,toSquare,1,0,0,0,P+side,capturedPiece,0);
                moveList.push_back(m);
                cout << "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;
            }



        }

    }

    //take enPassant square and check if any pawns are diagonally left or diagonally right of it
    //up or down direction depends on the side
    //if a current side pawn is there, that means it can attack the enPassant target square, so add it to moves
    int enPassSquare = board->enPassSquare;
    int diagonalOne = enPassSquare + 9 * ((!side) *-1);
    int diagonalTwo = enPassSquare + 7 * ((!side) *-1);
    unsigned long long optionOne = (1ULL << diagonalOne) & FileMasks[7-(7*side)];
    unsigned long long optionTwo = (1ULL << diagonalTwo) & FileMasks[7-(7*(!side))];

    if(optionOne){

        Move m(diagonalOne,enPassSquare,1,0,1,0,P+side,P+(!side),0);
        moveList.push_back(m);
        cout << "from: " << diagonalOne << "to: " << board->enPassSquare << "pawnEnPass" << " side: " << side << endl;
    }
    if(optionTwo){
        Move m(diagonalTwo,enPassSquare,1,0,1,0,P+side,P+(!side),0);
        moveList.push_back(m);
        cout << "from: " << diagonalTwo << "to: " << board->enPassSquare << "pawnEnPass" << " side: " << side << endl;
    }


    unsigned long long knights = bitboards[N + side];
    while (knights) {
        int fromSquare = popLSB(&knights);
        unsigned long long knightAllCaptures = t->knightMoveLookups[fromSquare] & enemyPieces;
        while (knightAllCaptures) {

            int toSquare = popLSB(&knightAllCaptures);
            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            Move m(fromSquare,toSquare,1,0,0,0,N+side,capturedPiece,0);
            moveList.push_back(m);

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

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            Move m(fromSquare,toSquare,1,0,0,0,B+side,capturedPiece,0);
            moveList.push_back(m);

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

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            Move m(fromSquare,toSquare,1,0,0,0,R+side,capturedPiece,0);
            moveList.push_back(m);
            cout << "from: " << fromSquare << "to: " << toSquare << "rook" << " side: " << side << endl;
        }
    }

    unsigned long long queens = bitboards[Q + side];
    while (queens) {
        int fromSquare = popLSB(&queens);
        unsigned long long queenAllCaptures = getQueenTargetFromBlockers(fromSquare, allPieces, t) & enemyPieces;

        while (queenAllCaptures) {
            int toSquare = popLSB(&queenAllCaptures);

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            Move m(fromSquare,toSquare,1,0,0,0,Q+side,capturedPiece,0);
            moveList.push_back(m);
            cout << "from: " << fromSquare << "to: " << toSquare << "queen" << " side: " << side << endl;
        }
    }


    unsigned long long king = bitboards[K + side];

    //assumption is that there will always have to be a single king on the board for each side
    int fromSquare = popLSB(&king);
    unsigned long long kingAllCaptures = t->kingMoveLookups[fromSquare] & enemyPieces;
    while (kingAllCaptures) {

        int toSquare = popLSB(&kingAllCaptures);

        unsigned long long toBit = 1ULL << toSquare;
        int capturedPiece = -1;

        //find out which piece is getting captured
        for(int piece = (2+(!side)); piece <13;piece+=2){

            if(bitboards[piece] & toBit){
                capturedPiece = piece;
                break;
            }

        }

        Move m(fromSquare,toSquare,1,0,0,0,K+side,capturedPiece,0);
        moveList.push_back(m);
        cout << "from: " << fromSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }

    return moveList;

}
void makeMove(Move m, Board* b){

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;

    unsigned long long movedPieceBitboard = b->bitboards[m.movedPiece];
    b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareFrom;

    if(m.capture){

        unsigned long long capturedPieceBitboard = b->bitboards[m.capturedPiece];

        if(m.enPassant){

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard ^ 1ULL << (squareTo +8 *((!b->sideToMove) *-1));
            return;
        }
        if(m.promotion){

            b->bitboards[m.capturedPiece] = capturedPieceBitboard ^ 1ULL << squareTo;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard | 1ULL << squareTo;
            return;
        }
        //normal captures
        else{
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard ^ 1ULL << squareTo;
            return;
        }

    }
    //non capture moves
    else{

        if(m.castle){
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            int squareDifference = squareTo - squareFrom;

            //queenside castle
            if(squareDifference ==-2){
                b->bitboards[R + b->sideToMove] ^= 1ULL << (squareTo-2);
                b->bitboards[R + b->sideToMove] |= 1ULL << (squareFrom-1);
            }
                //kingside castle
            else{
                b->bitboards[R + b->sideToMove] ^= 1ULL << (squareTo+1);
                b->bitboards[R + b->sideToMove] |= 1ULL << (squareFrom+1);
            }

            return;
        }
        if(m.promotion){
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard | 1ULL << squareTo;
            return;
        }
        //normal moves
        else{
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareTo;
        }
    }


}

void unmakeMove(Move m, Board* b){

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;

    unsigned long long movedPieceBitboard = b->bitboards[m.movedPiece];

    if(m.capture){

        unsigned long long capturedPieceBitboard = b->bitboards[m.capturedPiece];

        if(m.enPassant){

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard | 1ULL << (squareTo +8 *((!b->sideToMove) *-1));
            return;
        }
        if(m.promotion){

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard | 1ULL << squareTo;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard ^ 1ULL << squareTo;
            return;
        }
            //normal captures
        else{
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard | 1ULL << squareTo;
            return;
        }

    }
        //non capture moves
    else{

        if(m.castle){

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
            int squareDifference = squareTo - squareFrom;

            //queenside castle
            if(squareDifference ==-2){
                b->bitboards[R + b->sideToMove] |= 1ULL << (squareTo-2);
                b->bitboards[R + b->sideToMove] ^= 1ULL << (squareFrom-1);
            }
                //kingside castle
            else{
                b->bitboards[R + b->sideToMove] |= 1ULL << (squareTo+1);
                b->bitboards[R + b->sideToMove] ^= 1ULL << (squareFrom+1);
            }

            return;
        }
        if(m.promotion){
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard ^ 1ULL << squareTo;
            return;
        }
            //normal moves
        else{

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
        }
    }
}