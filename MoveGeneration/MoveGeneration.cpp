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


vector<Move> generateAllMoves(bool side,Board* board, TargetLibrary* t){

    vector<Move> moveList;

    unsigned long long* bitboards = board->bitboards;

    unsigned long long enemyPieces = bitboards[!side];
    unsigned long long friendlyPieces = bitboards[side];

    unsigned long long allPieces = enemyPieces | friendlyPieces;

    unsigned long long pawns = bitboards[P+side];

    while(pawns){
        int fromSquare = popLSB(&pawns);

        unsigned long long pawnSingle = t->pawnSinglePushLookups[side][fromSquare];
        unsigned long long pawnDouble = t->pawnDoublePushLookups[side][fromSquare];


        unsigned long long pawnMoveFirst = (pawnSingle & ~allPieces);
        unsigned long long pawnMoveSecond = (pawnDouble & ~allPieces);
        unsigned long long pawnMoveSecondLegal = pawnMoveSecond * (pawnMoveFirst && pawnMoveSecond);

        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;
        //unsigned long long pawnAllMoves = pawnMoveFirst | ((pawnMoveFirst && pawnMoveSecond) * pawnMoveSecond);

        int enPassSquare = board->enPassSquares.back();

        //enpass square of 64 means enpass is not available
        if(enPassSquare !=64){
            unsigned long long enPassBit = 1ULL << enPassSquare;

            if(pawnAttackTargets & enPassBit){
                Move m3(fromSquare,enPassSquare,0,1,0,1,0,P+side,P+!side,0);
                moveList.push_back(m3);
            }


        }

        while(pawnMoveFirst){
            int toSquare = popLSB(&pawnMoveFirst);

            //check for any promotions
            unsigned long long relevantRank = RankMasks[7+(-7*side)];

            if((1ULL<<toSquare) & relevantRank){
                Move m(fromSquare,toSquare,0,0,1,0,0,P+side,0,N+side);
                moveList.push_back(m);

                Move m1(fromSquare,toSquare,0,0,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);

                Move m2(fromSquare,toSquare,0,0,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);

                Move m3(fromSquare,toSquare,0,0,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);


            }
            else{
                Move m(fromSquare,toSquare,0,0,0,0,0,P+side,0,0);
                moveList.push_back(m);
            }


        }
        while(pawnMoveSecondLegal){
            int toSquare = popLSB(&pawnMoveSecondLegal);
            Move m(fromSquare,toSquare,1,0,0,0,0,P+side,0,0);
            moveList.push_back(m);
        }

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

                Move m1(fromSquare,toSquare,0,1,1,0,0,P+side,0,B+side);
                moveList.push_back(m1);

                Move m2(fromSquare,toSquare,0,1,1,0,0,P+side,0,R+side);
                moveList.push_back(m2);

                Move m3(fromSquare,toSquare,0,1,1,0,0,P+side,0,Q+side);
                moveList.push_back(m3);

            }
            else{
                Move m(fromSquare,toSquare,0,1,0,0,0,P+side,capturedPiece,0);
                moveList.push_back(m);
            }



        }

    }

    unsigned long long knights = bitboards[N+side];
    while(knights){
        int fromSquare = popLSB(&knights);

        unsigned long long knightAll = t->knightMoveLookups[fromSquare];
        unsigned long long knightQuietMoves = knightAll & ~allPieces;
        unsigned long long knightCaptureMoves = knightAll & enemyPieces;
        while(knightQuietMoves){

            int toSquare = popLSB(&knightQuietMoves);

            Move m(fromSquare,toSquare,0,0,0,0,0,N+side,0,0);
            moveList.push_back(m);
        }
        while(knightCaptureMoves){

            int toSquare = popLSB(&knightCaptureMoves);

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
        }
    }


    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int fromSquare = popLSB(&bishops);

        unsigned long long bishopAll = getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks);
        unsigned long long bishopQuietMoves = bishopAll  & ~allPieces;
        unsigned long long bishopCaptureMoves = bishopAll  & enemyPieces;

        while(bishopQuietMoves){

            int toSquare = popLSB(&bishopQuietMoves);
            Move m(fromSquare,toSquare,0,0,0,0,0,B+side,0,0);
            moveList.push_back(m);
        }
        while (bishopCaptureMoves) {

            int toSquare = popLSB(&bishopCaptureMoves);

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
        }

    }


    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int fromSquare = popLSB(&rooks);

        unsigned long long rookAll = getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks);
        unsigned long long rookQuietMoves = rookAll  & ~allPieces;
        unsigned long long rookCaptureMoves = rookAll & enemyPieces;

        while(rookQuietMoves){
            int toSquare = popLSB(&rookQuietMoves);

            Move m(fromSquare,toSquare,0,0,0,0,0,R+side,0,0);
            moveList.push_back(m);
        }
        while (rookCaptureMoves) {
            int toSquare = popLSB(&rookCaptureMoves);

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
        }
    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int fromSquare = popLSB(&queens);

        unsigned long long queenAll = getQueenTargetFromBlockers(fromSquare,allPieces,t);
        unsigned long long queenQuietMoves = queenAll & ~allPieces;
        unsigned long long queenCaptureMoves = queenAll & enemyPieces;

        while(queenQuietMoves){

            int toSquare = popLSB(&queenQuietMoves);
            Move m(fromSquare,toSquare,0,0,0,0,0,Q+side,0,0);
            moveList.push_back(m);
        }
        while (queenCaptureMoves) {
            int toSquare = popLSB(&queenCaptureMoves);

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
        }
    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int kingSquare = popLSB(&king);

    unsigned long long kingAll = t->kingMoveLookups[kingSquare];
    unsigned long long kingQuietMoves =  kingAll & ~allPieces;
    unsigned long long kingCaptureMoves = kingAll & enemyPieces;

    while(kingQuietMoves){

        int toSquare = popLSB(&kingQuietMoves);
        Move m(kingSquare,toSquare,0,0,0,0,0,K+side,0,0);
        moveList.push_back(m);

    }
    while (kingCaptureMoves) {

        int toSquare = popLSB(&kingCaptureMoves);

        unsigned long long toBit = 1ULL << toSquare;
        int capturedPiece = -1;

        //find out which piece is getting captured
        for(int piece = (2+(!side)); piece <13;piece+=2){

            if(bitboards[piece] & toBit){
                capturedPiece = piece;
                break;
            }

        }

        Move m(kingSquare,toSquare,0,1,0,0,0,K+side,capturedPiece,0);
        moveList.push_back(m);
    }


    unsigned int castleRights = board->castleRights.back();

    bool sideHasQueenSideCastle = castleRights & (1ULL<<(0+(side * 2))) && 1;
    bool sideHasKingSideCastle = castleRights & (1ULL<<(1+(side * 2))) && 1;

    unsigned long long enemyAttack = getAttackMask(!side,bitboards,t);

    if(sideHasKingSideCastle){


        int castleIndex = side * 2;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        if(!((enemyAttack & CastleSquares[castleIndex]) || ( CastleSquares[castleIndex] & (allPieces ^ (1ULL<<kingSquare)))) ){
            Move m(kingSquare,kingSquare+2,0,0,0,0,1,K+side,0,0);
            moveList.push_back(m);

        }



    }
    if(sideHasQueenSideCastle){

        int castleIndex = side * 2 + 1;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        if(!((enemyAttack & CastleSquares[castleIndex]) || ( CastleSquares[castleIndex] & (allPieces ^ (1ULL<<kingSquare)))) ){
            Move m(kingSquare,kingSquare-2,0,0,0,0,1,K+side,0,0);
            moveList.push_back(m);

        }

    }


    return moveList;

}


void makeMove(bool side,Move m, Board* b){

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;
    int movedPiece = m.movedPiece;
    int capturedPiece = m.capturedPiece;



    b->bitboards[movedPiece] ^= (1ULL << squareFrom);

    unsigned int castleRights = b->castleRights.back();
    if(movedPiece == R || movedPiece == r){

        //take away kingside castle since hfile rook moved
        if(1ULL<<squareFrom & RankMasks[7]){
            castleRights &= ~(1ULL<<(1+(side * 2)));
        }
        //take away queenside castle since afile rook moved
        if(1ULL<<squareFrom & RankMasks[0]){
            castleRights &= ~(1ULL<<(0+(side * 2)));

        }


    }
    //moved king, so take away both of side's castle rights
    if((movedPiece == K || movedPiece == k)){

        castleRights &= ~(1ULL<<(0+(side * 2)));
        castleRights &= ~(1ULL<<(1+(side * 2)));
    }


    if(m.capture){

        //capture makes enPassant unavailable and resets 50 move rule count
        b->enPassSquares.push_back(64);
        b->fiftyMoveRuleHalfMoves.push_back(0);


        if(capturedPiece == R || capturedPiece == r){

            //take away kingside castle since hfile rook captured
            if(1ULL<<squareTo & RankMasks[7]){
                castleRights &= ~(1ULL<<(1+((!side) * 2)));
            }
            //take away queenside castle since afile rook captured
            if(1ULL<<squareTo & RankMasks[0]){
                castleRights &= ~(1ULL<<(0+((!side) * 2)));
            }

        }

        if(m.enPassant){

            b->bitboards[movedPiece] |=1ULL << squareTo;
            b->bitboards[capturedPiece] ^= 1ULL << (squareTo + (8 + ((!side) * -16)));
        }
        if(m.promotion){

            b->bitboards[capturedPiece] ^= 1ULL << squareTo;
            b->bitboards[m.promotedTo] |= 1ULL << squareTo;
        }
        //normal captures
        if(!m.promotion && !m.enPassant) {
            b->bitboards[movedPiece] |= 1ULL << squareTo;
            b->bitboards[capturedPiece] ^= 1ULL << squareTo;
        }

    }
    //non capture moves
    else{

        //any type of pawn move resets 50 move rule count
        if(movedPiece == P || movedPiece == p){
            b->fiftyMoveRuleHalfMoves.push_back(0);
        }
        else{
            b->fiftyMoveRuleHalfMoves.push_back(b->fiftyMoveRuleHalfMoves.back()+1);
        }

        if(m.doublePush){
            unsigned int enPass = squareTo + (8 + ((!side) * -16));
            b->enPassSquares.push_back(enPass);
            b->bitboards[movedPiece] |= 1ULL << squareTo;

        }
        else{
            b->enPassSquares.push_back(64);

            if(m.castle){

                b->bitboards[movedPiece] |= 1ULL << squareTo;
                int squareDifference = squareTo - squareFrom;

                //queenside castle
                if(squareDifference ==-2){
                    b->bitboards[R + side] ^= 1ULL << (squareTo-2);
                    b->bitboards[R + side] |= 1ULL << (squareFrom-1);
                }
                    //kingside castle
                else{
                    b->bitboards[R + side] ^= 1ULL << (squareTo+1);
                    b->bitboards[R + side] |= 1ULL << (squareFrom+1);
                }

            }

            if(m.promotion){

                //unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
                b->bitboards[m.promotedTo] |= 1ULL << squareTo;
            }

                //normal moves
            if(!m.promotion && !m.castle){

                b->bitboards[movedPiece] |=  1ULL << squareTo;
            }



        }

    }

    b->castleRights.push_back(castleRights);


    //b->sideToMove = !b->sideToMove;
    //update allWhite bitboard
    b->bitboards[0] = 0ULL;
    for(int piece = 2; piece <14;piece+=2){

        b->bitboards[0] |= b->bitboards[piece];

    }

    //update allBlack bitboard
    b->bitboards[1] = 0ULL;
    for(int piece = 3; piece <14;piece+=2){

        b->bitboards[1] |= b->bitboards[piece];

    }


}

//make each move, check if the moving side's king is in check. If it is, then the move is illegal.
vector<Move> findLegalMoves(bool side, Board* board, vector<Move> allMoves, TargetLibrary* t){

    vector<Move> legalMoveList;


    for(int i = 0; i<allMoves.size(); i++){

        //cout<< "before" << endl;
        //displayWholeBoard(board);
        Move currMove = allMoves[i];
        makeMove(side, currMove,board);

        //currMove.toString();
        //cout<< "make" << endl;
        //displayWholeBoard(board);


        unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

        unsigned long long kingBit = board->bitboards[K+side];

        if(!(attackMask & kingBit)){
            legalMoveList.push_back(currMove);
        }

        unmakeMove(side,currMove,board);
        //cout<< "unmake" << endl;
        //displayWholeBoard(board);


    }

    return legalMoveList;
}

void unmakeMove(bool side, Move m, Board* b){

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;

    unsigned long long movedPieceBitboard = b->bitboards[m.movedPiece];

    if(m.capture){

        unsigned long long capturedPieceBitboard = b->bitboards[m.capturedPiece];

        if(m.enPassant){

            b->bitboards[m.movedPiece] |= 1ULL << squareFrom;
            b->bitboards[m.movedPiece] ^= 1ULL << squareTo;
            b->bitboards[m.capturedPiece] |= 1ULL << (squareTo + (8 + ((!side) * -16)));
        }
        else{
            if(m.promotion){

                b->bitboards[m.movedPiece]  |= 1ULL << squareFrom;
                b->bitboards[m.capturedPiece] |= 1ULL << squareTo;
                b->bitboards[m.promotedTo] ^= 1ULL << squareTo;
            }
                //normal captures
            else{
                b->bitboards[m.movedPiece] |= 1ULL << squareFrom;
                b->bitboards[m.movedPiece] ^= 1ULL << squareTo;
                b->bitboards[m.capturedPiece] |= 1ULL << squareTo;
            }
        }


    }
        //non capture moves
    else{

        if(m.castle){

            b->bitboards[m.movedPiece] |= 1ULL << squareFrom;
            b->bitboards[m.movedPiece] ^= 1ULL << squareTo;
            int squareDifference = squareTo - squareFrom;

            //queenside castle
            if(squareDifference ==-2){
                b->bitboards[R + side] |= 1ULL << (squareTo-2);
                b->bitboards[R + side] ^= 1ULL << (squareFrom-1);
            }
                //kingside castle
            else{
                b->bitboards[R + side] |= 1ULL << (squareTo+1);
                b->bitboards[R + side] ^= 1ULL << (squareFrom+1);
            }
        }else{
            if(m.promotion){
                b->bitboards[m.movedPiece] |= 1ULL << squareFrom;

                b->bitboards[m.promotedTo] ^= 1ULL << squareTo;
            }
                //normal moves
            else{

                b->bitboards[m.movedPiece] |= 1ULL << squareFrom;
                b->bitboards[m.movedPiece] ^= 1ULL << squareTo;
            }
        }

    }

    //update allWhite bitboard
    b->bitboards[0] = 0ULL;
    for(int piece = 2; piece <14;piece+=2){

        b->bitboards[0] |= b->bitboards[piece];

    }

    //update allBlack bitboard
    b->bitboards[1] = 0ULL;
    for(int piece = 3; piece <14;piece+=2){

        b->bitboards[1] |= b->bitboards[piece];

    }

    //b->sideToMove = !b->sideToMove;
    b->enPassSquares.pop_back();
    b->castleRights.pop_back();
    b->fiftyMoveRuleHalfMoves.pop_back();

}

unsigned long long Perft(int finishDepth, int printDepth, Board* board, TargetLibrary* t,bool side){

    unsigned long long moveCount =0;

    if(finishDepth == 0){
        return 1ULL;
    }

    vector<Move> ms = generateAllMoves(side,board,t);
    vector<Move> legals = findLegalMoves(side,board,ms,t);

    for(Move m : legals){

        //cout << "BEFORE==========================================" << endl;
        //displayWholeBoard(board);

        makeMove(side,m,board);

        //cout << "MAKE============================================" << endl;
        //m.toString();
        //displayWholeBoard(board);


        unsigned long ct = Perft(finishDepth - 1,printDepth, board,t,!side);
        if(printDepth == finishDepth){
            m.toString();
            m.toUCI();
            cout << ct << " nodes" << endl;
        }
        moveCount += ct;
        unmakeMove(side,m,board);

        //cout << "UNMAKE==========================================" << endl;
        //displayWholeBoard(board);
    }

    return moveCount;


}
void generateMovesCertainDepth(int depth,Board* board, TargetLibrary* t,bool side){

    if(depth ==0){
        return;
    }

    vector<Move> ms = generateAllMoves(side,board,t);
    vector<Move> legals = findLegalMoves(side,board,ms,t);
    displayWholeBoard(board);

    for(Move m : legals){

        makeMove(side,m,board);
        displayWholeBoard(board);
        generateMovesCertainDepth(depth-1,board,t,!side);
        unmakeMove(side,m,board);
        displayWholeBoard(board);
    }

    return;
}
