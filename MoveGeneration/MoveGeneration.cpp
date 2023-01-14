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
        unsigned long long pawnMoveSecondLegal = pawnMoveSecond * (pawnMoveFirst && pawnMoveSecond);
        //unsigned long long pawnAllMoves = pawnMoveFirst | ((pawnMoveFirst && pawnMoveSecond) * pawnMoveSecond);

        while(pawnMoveFirst){
            int toSquare = popLSB(&pawnMoveFirst);

            //check for any promotions
            unsigned long long relevantRank = RankMasks[7+(-7*side)];

            if((1ULL<<toSquare) & relevantRank){
                Move m(fromSquare,toSquare,0,0,1,0,0,P+side,0,N+side);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m1(fromSquare,toSquare,0,0,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m2(fromSquare,toSquare,0,0,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m3(fromSquare,toSquare,0,0,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;

            }
            else{
                Move m(fromSquare,toSquare,0,0,0,0,0,P+side,0,0);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;
            }


        }
        while(pawnMoveSecondLegal){
            int toSquare = popLSB(&pawnMoveSecondLegal);
            Move m(fromSquare,toSquare,1,0,0,0,0,P+side,0,0);
            moveList.push_back(m);
            cout<< "from: " << fromSquare << "to: " << toSquare << "pawnDouble" << " side: " << side << endl;
        }

    }


    unsigned long long knights = bitboards[N+side];
    while(knights){
        int fromSquare = popLSB(&knights);
        unsigned long long knightAllMoves = t->knightMoveLookups[fromSquare] & ~allPieces;
        while(knightAllMoves){

            int toSquare = popLSB(&knightAllMoves);

            Move m(fromSquare,toSquare,0,0,0,0,0,N+side,0,0);
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
            Move m(fromSquare,toSquare,0,0,0,0,0,B+side,0,0);
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

            Move m(fromSquare,toSquare,0,0,0,0,0,R+side,0,0);
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
            Move m(fromSquare,toSquare,0,0,0,0,0,Q+side,0,0);
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
        Move m(kingSquare,toSquare,0,0,0,0,0,K+side,0,0);
        moveList.push_back(m);
        cout<< "from: " << kingSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }

    unsigned int castleRights = board->castleRights.back();

    bool sideHasQueenSideCastle = castleRights & (1ULL<<(0+(side * 2))) && 1;
    bool sideHasKingSideCastle = castleRights & (1ULL<<(1+(side * 2))) && 1;

    if(sideHasKingSideCastle){

        //shift king left 2 and shift kingSideRook right 2

        Move m(kingSquare,kingSquare+2,0,0,0,0,1,K+side,0,0);
        moveList.push_back(m);
        cout<< "from: " << kingSquare << "to: " << kingSquare+2 << "kingCastle" << " side: " << side << endl;

    }
    if(sideHasQueenSideCastle){

        //shift king right 2 and shift queenSideRook left 3

        Move m(kingSquare,kingSquare-2,0,0,0,0,1,K+side,0,0);
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
                Move m(fromSquare,toSquare,0,1,1,0,0,P+side,0,N+side);
                moveList.push_back(m);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m1(fromSquare,toSquare,0,1,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m2(fromSquare,toSquare,0,1,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;
                Move m3(fromSquare,toSquare,0,1,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);
                cout<< "from: " << fromSquare << "to: " << toSquare << "pawnPromo" << " side: " << side << endl;

            }
            else{
                Move m(fromSquare,toSquare,0,1,0,0,0,P+side,capturedPiece,0);
                moveList.push_back(m);
                cout << "from: " << fromSquare << "to: " << toSquare << "pawn" << " side: " << side << endl;
            }



        }

    }

    //take enPassant square and check if any pawns are diagonally left or diagonally right of it
    //up or down direction depends on the side
    //if a current side pawn is there, that means it can attack the enPassant target square, so add it to moves
    int enPassSquare = board->enPassSquares.back();

    //if enPassSquare = 64(enPassant not available) this is 0
    unsigned long long enPassBit = 1ULL << enPassSquare;

    int diagonalOne = enPassSquare + 9 * ((!side) *-1);
    int diagonalTwo = enPassSquare + 7 * ((!side) *-1);
    unsigned long long optionOne = (1ULL << diagonalOne) & FileMasks[7-(7*side)];
    unsigned long long optionTwo = (1ULL << diagonalTwo) & FileMasks[7-(7*(!side))];

    if(optionOne && enPassBit){

        Move m(diagonalOne,enPassSquare,0,1,0,1,0,P+side,P+(!side),0);
        moveList.push_back(m);
        cout << "from: " << diagonalOne << "to: " << enPassSquare << "pawnEnPass" << " side: " << side << endl;
    }
    if(optionTwo && enPassBit){
        Move m(diagonalTwo,enPassSquare,0,1,0,1,0,P+side,P+(!side),0);
        moveList.push_back(m);
        cout << "from: " << diagonalTwo << "to: " << enPassSquare << "pawnEnPass" << " side: " << side << endl;
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

            Move m(fromSquare,toSquare,0,1,0,0,0,N+side,capturedPiece,0);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,B+side,capturedPiece,0);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,R+side,capturedPiece,0);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,Q+side,capturedPiece,0);
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

        Move m(fromSquare,toSquare,0,1,0,0,0,K+side,capturedPiece,0);
        moveList.push_back(m);
        cout << "from: " << fromSquare << "to: " << toSquare << "king" << " side: " << side << endl;
    }

    return moveList;

}
void makeMove(Move m, Board* b){

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;
    int movedPiece = m.movedPiece;
    int capturedPiece = m.capturedPiece;

    unsigned long long movedPieceBitboard = b->bitboards[movedPiece];
    b->bitboards[movedPiece] = movedPieceBitboard ^ 1ULL << squareFrom;



    unsigned int castleRights = b->castleRights.back();
    if(movedPiece == R || movedPiece == r){

        //take away kingside castle since hfile rook moved
        if(1ULL<<squareFrom & RankMasks[7]){
            castleRights &= ~(1ULL<<(1+(b->sideToMove * 2)));
            b->castleRights.push_back(castleRights);
        }
        //take away queenside castle since afile rook moved
        if(1ULL<<squareFrom & RankMasks[0]){
            castleRights &= ~(1ULL<<(0+(b->sideToMove * 2)));
            b->castleRights.push_back(castleRights);
        }


    }
    //moved king, so take away both of side's castle rights
    if((movedPiece == K || movedPiece == k)){

        castleRights &= ~(1ULL<<(0+(b->sideToMove * 2)));
        castleRights &= ~(1ULL<<(1+(b->sideToMove * 2)));

        b->castleRights.push_back(castleRights);
    }


    if(m.capture){

        //capture makes enPassant unavailable and resets 50 move rule count
        b->enPassSquares.push_back(64);
        b->fiftyMoveRuleHalfMoves.push_back(0);

        unsigned long long capturedPieceBitboard = b->bitboards[capturedPiece];

        if(m.enPassant){

            b->bitboards[movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            b->bitboards[capturedPiece] = capturedPieceBitboard ^ 1ULL << (squareTo +8 *((!b->sideToMove) *-1));
        }
        if(m.promotion){

            b->bitboards[capturedPiece] = capturedPieceBitboard ^ 1ULL << squareTo;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard | 1ULL << squareTo;
        }
        //normal captures
        if(!m.promotion && !m.enPassant) {
            b->bitboards[movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            b->bitboards[capturedPiece] = capturedPieceBitboard ^ 1ULL << squareTo;
        }

    }
    //non capture moves
    else{

        //any type of pawn move resets 50 move rule count
        if(movedPiece == P || movedPiece == p){
            b->fiftyMoveRuleHalfMoves.push_back(0);
        }

        if(m.doublePush){
            unsigned int enPass = squareTo + (8 * ((!b->sideToMove) *-1));
            b->enPassSquares.push_back(enPass);
            b->bitboards[movedPiece] = movedPieceBitboard | 1ULL << squareTo;

        }
        else{
            b->enPassSquares.push_back(64);


            if(m.castle){
                b->enPassSquares.push_back(64);

                b->bitboards[movedPiece] = movedPieceBitboard | 1ULL << squareTo;
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

            }

            if(m.promotion){
                b->enPassSquares.push_back(64);
                unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
                b->bitboards[m.promotedTo] = promotedPieceBitboard | 1ULL << squareTo;
            }

                //normal moves
            if(!m.promotion && !m.castle){

                b->bitboards[movedPiece] = movedPieceBitboard | 1ULL << squareTo;
            }



        }

    }

    b->sideToMove = !b->sideToMove;
    //update allWhite bitboard
    b->bitboards[0] = 0ULL;
    for(int piece = 2; piece <13;piece+=2){

        b->bitboards[0] |= b->bitboards[piece];

    }

    //update allBlack bitboard
    b->bitboards[1] = 0ULL;
    for(int piece = 3; piece <13;piece+=2){

        b->bitboards[1] |= b->bitboards[piece];

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
        }
        if(m.promotion){

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard | 1ULL << squareTo;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard ^ 1ULL << squareTo;
        }
            //normal captures
        else{
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
            b->bitboards[m.capturedPiece] = capturedPieceBitboard | 1ULL << squareTo;
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
        }
        if(m.promotion){
            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
            b->bitboards[m.promotedTo] = promotedPieceBitboard ^ 1ULL << squareTo;
        }
            //normal moves
        else{

            b->bitboards[m.movedPiece] = movedPieceBitboard | 1ULL << squareFrom;
            b->bitboards[m.movedPiece] = movedPieceBitboard ^ 1ULL << squareTo;
        }
    }

    //update allWhite bitboard
    b->bitboards[0] = 0ULL;
    for(int piece = 2; piece <13;piece+=2){

        b->bitboards[0] |= b->bitboards[piece];

    }

    //update allBlack bitboard
    b->bitboards[1] = 0ULL;
    for(int piece = 3; piece <13;piece+=2){

        b->bitboards[1] |= b->bitboards[piece];

    }

    b->sideToMove = !b->sideToMove;
    b->enPassSquares.pop_back();
    b->castleRights.pop_back();
    b->fiftyMoveRuleHalfMoves.pop_back();
}