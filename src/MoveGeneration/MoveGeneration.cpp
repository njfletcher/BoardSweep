//
// Created by nflet on 1/5/2023.
//
#include "MoveGeneration.h"
#include "../BitUtil.h"
#include "TargetGeneration.h"
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

//white = 0 black =1;
/*getAttackMask-
 * given a side, bitboard array, and target library
 * returns a bitboard with set bits representing all possible squares
 * that side can pseudo-legally attack
 */
unsigned long long getAttackMask(bool side,const unsigned long long* bitboards, LookupLibrary* t){

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

void generateAllCaptures(Board* board, LookupLibrary* t, Move* list, int* moveCount){


    bool side = board->sideToMove;

    unsigned long long* bitboards = board->bitboards;

    unsigned long long enemyPieces = bitboards[!side];
    unsigned long long friendlyPieces = bitboards[side];

    unsigned long long allPieces = enemyPieces | friendlyPieces;

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int fromSquare = popLSB(&queens);

        unsigned long long queenAll = getQueenTargetFromBlockers(fromSquare,allPieces,t);
        unsigned long long queenCaptureMoves = queenAll & enemyPieces;

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

            Move m(fromSquare,toSquare,false,true,false,false,false,Q+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;

        }
    }

    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int fromSquare = popLSB(&rooks);

        unsigned long long rookAll = getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks);
        unsigned long long rookCaptureMoves = rookAll & enemyPieces;

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

            Move m(fromSquare,toSquare,false,true,false,false,false,R+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }
    }
    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int fromSquare = popLSB(&bishops);

        unsigned long long bishopAll = getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks);
        unsigned long long bishopCaptureMoves = bishopAll  & enemyPieces;

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

            Move m(fromSquare,toSquare,false,true,false,false,false,B+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

    }


    unsigned long long knights = bitboards[N+side];
    while(knights) {
        int fromSquare = popLSB(&knights);

        unsigned long long knightAll = t->knightMoveLookups[fromSquare];
        unsigned long long knightCaptureMoves = knightAll & enemyPieces;

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

            Move m(fromSquare,toSquare,false,true,false,false,false,N+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }


    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int kingSquare = popLSB(&king);

    unsigned long long kingAll = t->kingMoveLookups[kingSquare];
    unsigned long long kingCaptureMoves = kingAll & enemyPieces;

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

        Move m(kingSquare,toSquare,false,true,false,false,false,K+side,capturedPiece,0);
        //moves->push_back(m);
        //addToMoveList(start,&m);
        list[*moveCount] = m;
        *moveCount = *moveCount + 1;
    }

    unsigned long long pawns = bitboards[P+side];

    while(pawns){

        int fromSquare = popLSB(&pawns);

        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;

        //int enPassSquare = board->enPassSquares.back();
        int enPassSquare = board->enPassSquares[board->currentDepth];

        //enpass square of 64 means enpass is not available
        if(enPassSquare !=64){
            unsigned long long enPassBit = 1ULL << enPassSquare;

            if(pawnAttackTargets & enPassBit){
                Move m3(fromSquare,enPassSquare,false,true,false,true,false,P+side,P+!side,0);
                //moves->push_back(m3);
                //addToMoveList(start,&m3);
                list[*moveCount] = m3;
                *moveCount = *moveCount + 1;
            }


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
                Move m(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,N+side);
                //moves->push_back(m);
                //addToMoveList(start,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;

                Move m1(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,B+side);
                //moves->push_back(m1);
                //addToMoveList(start,&m1);
                list[*moveCount] = m1;
                *moveCount = *moveCount + 1;

                Move m2(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,R+side);
                //moves->push_back(m2);
                //addToMoveList(start,&m2);
                list[*moveCount] = m2;
                *moveCount = *moveCount + 1;

                Move m3(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,Q+side);
                //moves->push_back(m3);
                //addToMoveList(start,&m3);
                list[*moveCount] = m3;
                *moveCount = *moveCount + 1;

            }
            else{
                Move m(fromSquare,toSquare,false,true,false,false,false,P+side,capturedPiece,0);
                //moves->push_back(m);
                //addToMoveList(start,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;
            }



        }



    }

}
bool checkIfSquareAttacked(bool side, Board* board, LookupLibrary* t, int square){


    unsigned long long* bitboards = board->bitboards;
    int kingSquare = square;

    bool flippedSide = !side;

    unsigned long long whitePieces = bitboards[0];
    unsigned long long blackPieces = bitboards[1];
    unsigned long long allPieces = whitePieces | blackPieces;

    unsigned long long enemyPawns = bitboards[P+flippedSide];
    unsigned long long pAttack = t->pawnAttackLookups[side][kingSquare];
    if(pAttack & enemyPawns)return true;

    unsigned long long enemyKnights = bitboards[N+flippedSide];
    unsigned long long nAttack = t->knightMoveLookups[kingSquare];
    if(nAttack & enemyKnights) return true;

    unsigned long long enemyBishops = bitboards[B+flippedSide];
    unsigned long long bAttacks = getBishopTargetFromBlockers(kingSquare,allPieces  & t->bishopTargetLookups[kingSquare],t->bishopMagicAttacks);
    if(bAttacks & enemyBishops) return true;

    unsigned long long enemyRooks = bitboards[R+flippedSide];
    unsigned long long rAttacks = getRookTargetFromBlockers(kingSquare,allPieces & t->rookTargetLookups[kingSquare],t->rookMagicAttacks);
    if(rAttacks & enemyRooks) return true;

    unsigned long long enemyQueens = bitboards[Q+flippedSide];
    unsigned long long qAttacks = getQueenTargetFromBlockers(kingSquare,allPieces,t);
    if(qAttacks & enemyQueens) return true;

    unsigned long long enemyKing = bitboards[K+flippedSide];
    unsigned long long kAttacks = t->kingMoveLookups[kingSquare];
    if(kAttacks & enemyKing) return true;

    return false;


}
bool checkIfInCheck(bool side, Board* board, LookupLibrary* t){

    unsigned long long kingBitBoard = board->bitboards[K+side];
    //unsigned long long kingBitBoard = kingBoard;
    unsigned long long* bitboards = board->bitboards;
    int kingSquare = popLSB(&kingBitBoard);
    kingBitBoard = 1ULL << kingSquare;

    bool flippedSide = !side;

    unsigned long long whitePieces = bitboards[0];
    unsigned long long blackPieces = bitboards[1];
    unsigned long long allPieces = whitePieces | blackPieces;

    unsigned long long enemyPawns = bitboards[P+flippedSide];
    unsigned long long pAttack = t->pawnAttackLookups[side][kingSquare];
    if(pAttack & enemyPawns)return true;

    unsigned long long enemyKnights = bitboards[N+flippedSide];
    unsigned long long nAttack = t->knightMoveLookups[kingSquare];
    if(nAttack & enemyKnights) return true;

    unsigned long long enemyBishops = bitboards[B+flippedSide];
    unsigned long long bAttacks = getBishopTargetFromBlockers(kingSquare,allPieces  & t->bishopTargetLookups[kingSquare],t->bishopMagicAttacks);
    if(bAttacks & enemyBishops) return true;

    unsigned long long enemyRooks = bitboards[R+flippedSide];
    unsigned long long rAttacks = getRookTargetFromBlockers(kingSquare,allPieces & t->rookTargetLookups[kingSquare],t->rookMagicAttacks);
    if(rAttacks & enemyRooks) return true;

    unsigned long long enemyQueens = bitboards[Q+flippedSide];
    unsigned long long qAttacks = getQueenTargetFromBlockers(kingSquare,allPieces,t);
    if(qAttacks & enemyQueens) return true;

    unsigned long long enemyKing = bitboards[K+flippedSide];
    unsigned long long kAttacks = t->kingMoveLookups[kingSquare];
    if(kAttacks & enemyKing) return true;

    return false;


}
/* generateAllMoves-
 * given a side, bitboard array, and target library
 * returns vector of all psuedo-legal moves(attack,normal, and special)
 * that a side can make
 */
unsigned long long* generateAllMoves(Board* board, LookupLibrary* t, int* moveCount, unsigned long long* list){

    bool side = board->sideToMove;
    unsigned long long* bitboards = board->bitboards;

    unsigned long long enemyPieces = bitboards[!side];
    unsigned long long friendlyPieces = bitboards[side];

    unsigned long long allPieces = enemyPieces | friendlyPieces;

    unsigned long long pawns = bitboards[P+side];

    while(pawns){
        unsigned int fromSquare = popLSB(&pawns);

        unsigned long long pawnSingle = t->pawnSinglePushLookups[side][fromSquare];
        unsigned long long pawnDouble = t->pawnDoublePushLookups[side][fromSquare];


        unsigned long long pawnMoveFirst = (pawnSingle & ~allPieces);
        unsigned long long pawnMoveSecond = (pawnDouble & ~allPieces);
        unsigned long long pawnMoveSecondLegal = pawnMoveSecond * (pawnMoveFirst && pawnMoveSecond);
        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;

        //int enPassSquare = board->enPassSquares.back();
        unsigned int enPassSquare = board->enPassSquares[board->currentDepth];

        //enpass square of 64 means enpass is not available
        if(enPassSquare !=64){
            unsigned long long enPassBit = 1ULL << enPassSquare;

            if(pawnAttackTargets & enPassBit){
                //Move m3(fromSquare,enPassSquare,false,true,false,true,false,P+side,P+!side,0);
                unsigned long long m3 = packageMove(fromSquare,enPassSquare,false,true,false,true,false,P+side,P+!side,0);
                //moves->push_back(m3);
                //addToMoveList(start,&m3);
                list[*moveCount] = m3;
                *moveCount = *moveCount + 1;
            }


        }

        while (pawnNormalCaptures) {
            unsigned int toSquare = popLSB(&pawnNormalCaptures);
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
                //Move m(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,N+side);
                unsigned long long m = packageMove(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,N+side);
                //moves->push_back(m);
                //addToMoveList(start,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;

                //Move m1(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,B+side);
                unsigned long long m1 = packageMove(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,B+side);
                //moves->push_back(m1);
                //addToMoveList(start,&m1);
                list[*moveCount] = m1;
                *moveCount = *moveCount + 1;

                //Move m2(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,R+side);
                unsigned long long m2 = packageMove(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,R+side);
                //moves->push_back(m2);
                //addToMoveList(start,&m2);
                list[*moveCount] = m2;
                *moveCount = *moveCount + 1;

                //Move m3(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,Q+side);
                unsigned long long m3 = packageMove(fromSquare,toSquare,false,true,true,false,false,P+side,capturedPiece,Q+side);
                //moves->push_back(m3);
                //addToMoveList(start,&m3);
                list[*moveCount] = m3;
                *moveCount = *moveCount + 1;

            }
            else{
                //Move m(fromSquare,toSquare,false,true,false,false,false,P+side,capturedPiece,0);
                unsigned long long m = packageMove(fromSquare,toSquare,false,true,false,false,false,P+side,capturedPiece,0);
                //moves->push_back(m);
                //addToMoveList(start,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;
            }



        }

        while(pawnMoveFirst){
            unsigned int toSquare = popLSB(&pawnMoveFirst);

            //check for any promotions
            unsigned long long relevantRank = RankMasks[7+(-7*side)];

            if((1ULL<<toSquare) & relevantRank){
                //Move m(fromSquare,toSquare,false,false,true,false,false,P+side,0,N+side);
                unsigned long long m = packageMove(fromSquare,toSquare,false,false,true,false,false,P+side,0,N+side);
                //moveList.push_back(m);
                //addToMoveList(listStart,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;

                //Move m1(fromSquare,toSquare,false,false,true,false,false,P+side,0,B+side);
                unsigned long long m1 = packageMove(fromSquare,toSquare,false,false,true,false,false,P+side,0,B+side);
                //moveList.push_back(m1);
                //addToMoveList(listStart,&m1);
                list[*moveCount] = m1;
                *moveCount = *moveCount + 1;

                //Move m2(fromSquare,toSquare,false,false,true,false,false,P+side,0,R+side);
                unsigned long long m2 = packageMove(fromSquare,toSquare,false,false,true,false,false,P+side,0,R+side);
                //moveList.push_back(m2);
                //addToMoveList(listStart,&m2);
                list[*moveCount] = m2;
                *moveCount = *moveCount + 1;

                //Move m3(fromSquare,toSquare,false,false,true,false,false,P+side,0,Q+side);
                unsigned long long m3 = packageMove(fromSquare,toSquare,false,false,true,false,false,P+side,0,Q+side);
                //moveList.push_back(m3);
                //addToMoveList(listStart,&m3);
                list[*moveCount] = m3;
                *moveCount = *moveCount + 1;


            }
            else{
                //Move m(fromSquare,toSquare,false,false,false,false,false,P+side,0,0);
                unsigned long long m = packageMove(fromSquare,toSquare,false,false,false,false,false,P+side,0,0);
                //moveList.push_back(m);
                //addToMoveList(listStart,&m);
                list[*moveCount] = m;
                *moveCount = *moveCount + 1;
            }


        }
        while(pawnMoveSecondLegal){
            unsigned int toSquare = popLSB(&pawnMoveSecondLegal);
            //Move m(fromSquare,toSquare,true,false,false,false,false,P+side,0,0);
            unsigned long long m = packageMove(fromSquare,toSquare,true,false,false,false,false,P+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

    }

    unsigned long long knights = bitboards[N+side];
    while(knights){
        unsigned int fromSquare = popLSB(&knights);

        unsigned long long knightAll = t->knightMoveLookups[fromSquare];
        unsigned long long knightQuietMoves = knightAll & ~allPieces;
        unsigned long long knightCaptureMoves = knightAll & enemyPieces;

        while(knightCaptureMoves){

            unsigned int toSquare = popLSB(&knightCaptureMoves);

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            //Move m(fromSquare,toSquare,false,true,false,false,false,N+side,capturedPiece,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,true,false,false,false,N+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }
        while(knightQuietMoves){

            unsigned int toSquare = popLSB(&knightQuietMoves);

            //Move m(fromSquare,toSquare,false,false,false,false,false,N+side,0,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,false,false,false,false,N+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

    }


    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        unsigned int fromSquare = popLSB(&bishops);

        unsigned long long bishopAll = getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks);
        unsigned long long bishopQuietMoves = bishopAll  & ~allPieces;
        unsigned long long bishopCaptureMoves = bishopAll  & enemyPieces;

        while (bishopCaptureMoves) {

            unsigned int toSquare = popLSB(&bishopCaptureMoves);

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            //Move m(fromSquare,toSquare,false,true,false,false,false,B+side,capturedPiece,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,true,false,false,false,B+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

        while(bishopQuietMoves){

            unsigned int toSquare = popLSB(&bishopQuietMoves);
            //Move m(fromSquare,toSquare,false,false,false,false,false,B+side,0,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,false,false,false,false,B+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }


    }


    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        unsigned int fromSquare = popLSB(&rooks);

        unsigned long long rookAll = getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks);
        unsigned long long rookQuietMoves = rookAll  & ~allPieces;
        unsigned long long rookCaptureMoves = rookAll & enemyPieces;

        while (rookCaptureMoves) {
            unsigned int toSquare = popLSB(&rookCaptureMoves);

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            //Move m(fromSquare,toSquare,false,true,false,false,false,R+side,capturedPiece,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,true,false,false,false,R+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

        while(rookQuietMoves){
            unsigned int toSquare = popLSB(&rookQuietMoves);

            //Move m(fromSquare,toSquare,false,false,false,false,false,R+side,0,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,false,false,false,false,R+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        unsigned int fromSquare = popLSB(&queens);

        unsigned long long queenAll = getQueenTargetFromBlockers(fromSquare,allPieces,t);
        unsigned long long queenQuietMoves = queenAll & ~allPieces;
        //unsigned long long queenCaptureMoves = queenAll & enemyPieces;
        unsigned long long queenCaptureMoves = queenAll & enemyPieces;

        while (queenCaptureMoves) {
            unsigned int toSquare = popLSB(&queenCaptureMoves);

            unsigned long long toBit = 1ULL << toSquare;
            int capturedPiece = -1;

            //find out which piece is getting captured
            for(int piece = (2+(!side)); piece <13;piece+=2){

                if(bitboards[piece] & toBit){
                    capturedPiece = piece;
                    break;
                }

            }

            //Move m(fromSquare,toSquare,false,true,false,false,false,Q+side,capturedPiece,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,true,false,false,false,Q+side,capturedPiece,0);
            //moves->push_back(m);
            //addToMoveList(start,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;

        }

        while(queenQuietMoves){

            unsigned int toSquare = popLSB(&queenQuietMoves);
            //Move m(fromSquare,toSquare,false,false,false,false,false,Q+side,0,0);
            unsigned long long m = packageMove(fromSquare,toSquare,false,false,false,false,false,Q+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;
        }

    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    unsigned int kingSquare = popLSB(&king);

    unsigned long long kingAll = t->kingMoveLookups[kingSquare];
    unsigned long long kingQuietMoves =  kingAll & ~allPieces;
    unsigned long long kingCaptureMoves = kingAll & enemyPieces;

    while (kingCaptureMoves) {

        unsigned int toSquare = popLSB(&kingCaptureMoves);

        unsigned long long toBit = 1ULL << toSquare;
        int capturedPiece = -1;

        //find out which piece is getting captured
        for(int piece = (2+(!side)); piece <13;piece+=2){

            if(bitboards[piece] & toBit){
                capturedPiece = piece;
                break;
            }

        }

        //Move m(kingSquare,toSquare,false,true,false,false,false,K+side,capturedPiece,0);
        unsigned long long m = packageMove(kingSquare,toSquare,false,true,false,false,false,K+side,capturedPiece,0);
        //moves->push_back(m);
        //addToMoveList(start,&m);
        list[*moveCount] = m;
        *moveCount = *moveCount + 1;
    }

    while(kingQuietMoves){

        unsigned int toSquare = popLSB(&kingQuietMoves);
        //Move m(kingSquare,toSquare,false,false,false,false,false,K+side,0,0);
        unsigned long long m = packageMove(kingSquare,toSquare,false,false,false,false,false,K+side,0,0);
        //moveList.push_back(m);
        //addToMoveList(listStart,&m);
        list[*moveCount] = m;
        *moveCount = *moveCount + 1;

    }


    //unsigned int castleRights = board->castleRights.back();
    unsigned int castleRights = board->castleRights[board->currentDepth];

    bool sideHasQueenSideCastle = (castleRights & (1ULL << (0 + (side * 2)))) != 0;
    bool sideHasKingSideCastle = (castleRights & (1ULL << (1 + (side * 2)))) != 0;



    //unsigned long long enemyAttack = getAttackMask(!side,bitboards,t);

    if(sideHasKingSideCastle){

        unsigned int kingSquare1 = 4 + 56 * side;
        unsigned int kingSquare2 = 5 + 56 * side;


        //enemyAttack & CastleCheckSquares[castleIndex]
        int castleIndex = side * 2;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        if(!((checkIfSquareAttacked(side,board,t,kingSquare1) || (checkIfSquareAttacked(side,board,t,kingSquare2))) || ( CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL<<kingSquare))))){
            //Move m(kingSquare,kingSquare+2,false,false,false,false,true,K+side,0,0);
            unsigned long long m = packageMove(kingSquare,kingSquare+2,false,false,false,false,true,K+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;

        }



    }
    if(sideHasQueenSideCastle){

        unsigned int queenSquare1 = 4 + 56 * side;
        unsigned int queenSquare2 = 3 + 56 * side;

        int castleIndex = side * 2 + 1;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        //enemyAttack & CastleCheckSquares[castleIndex]
        if(!(((checkIfSquareAttacked(side,board,t,queenSquare1) || (checkIfSquareAttacked(side,board,t,queenSquare2))) || ( CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL<<kingSquare))))) ){
            //Move m(kingSquare,kingSquare-2,false,false,false,false,true,K+side,0,0);
            unsigned long long m = packageMove(kingSquare,kingSquare-2,false,false,false,false,true,K+side,0,0);
            //moveList.push_back(m);
            //addToMoveList(listStart,&m);
            list[*moveCount] = m;
            *moveCount = *moveCount + 1;

        }

    }


    return list;

}

/* makeMove-
 * given a side, move, and board
 * updates the board to reflect the move being made
 */

bool makeMove(unsigned long long m, Board* b, LookupLibrary* t){

    //unsigned long long position = b->currentPosition;

    int currentDepth = b->currentDepth;

    //int squareFrom = m.squareFrom;
    unsigned int squareFrom = m & 0b111111;
    //int squareTo = m.squareTo;
    unsigned int squareTo = (m & (0b111111 << 6)) >> 6;
    //int movedPiece = m.movedPiece;
    unsigned int movedPiece = (m & (0b1111 << 17)) >>17;
    //int capturedPiece = m.capturedPiece;
    unsigned int capturedPiece = (m & (0b1111 << 21)) >>21;
    //unsigned int promotedPiece = m.promotedTo;
    unsigned int promotedPiece = (m & (0b1111 << 25)) >> 25;

    unsigned long long isDoubleP = (m & (0b1 <<12));
    unsigned long long isCapture = (m & (0b1 << 13));
    unsigned long long isPromotion = (m& (0b1 <<14));
    unsigned long long isEnPass = (m& (0b1 <<15));
    unsigned long long isCastle = (m&(0b1 <<16));
    bool side = b->sideToMove;

    //int enemyIndex = 0 + !side;
    //int friendlyIndex = 0 + side;

    unsigned long long squareFromBit = 1ULL << squareFrom;
    b->bitboards[movedPiece] ^= squareFromBit;
    unsigned long long squareToBit = 1ULL << squareTo;
    //b->bitboards[movedPiece] ^= squareToBit;
    //b->bitboards[friendlyIndex] ^= squareFromBit;

    //position^= t->zobristPieces[movedPiece][squareFrom];

    //position^= t->zobristEnPass[b->enPassSquares.back()];

    //unsigned int castleRights = b->castleRights.back();
    unsigned int castleRights = b->castleRights[currentDepth];
    //position^= t->zobristCastles[castleRights];

    if(movedPiece == R){
        if(squareFrom == 0){
            castleRights &= ~(1ULL);
        }
        if(squareFrom == 7){
            castleRights &= ~(1ULL<<1);
        }
    }
    if(movedPiece ==r){
        if(squareFrom == 56){
            castleRights &= ~(1ULL<<2);
        }
        if(squareFrom == 63){
            castleRights &= ~(1ULL<<3);
        }
    }
    //moved king, so take away both of side's castle rights
    if((movedPiece == K || movedPiece == k)){

        castleRights &= ~(1ULL<<(0+(side * 2)));
        castleRights &= ~(1ULL<<(1+(side * 2)));
    }
    if(movedPiece == P || movedPiece == p){
        //b->fiftyMoveRuleHalfMoves.push_back(0);
        b->fiftyMoveRuleHalfMoves[currentDepth+1] = 0;
    }
    else{
        //b->fiftyMoveRuleHalfMoves.push_back(b->fiftyMoveRuleHalfMoves.back()+1);
        b->fiftyMoveRuleHalfMoves[currentDepth +1] = b->fiftyMoveRuleHalfMoves[currentDepth] + 1;
    }



    if(isCapture){

        //capture makes enPassant unavailable and resets 50 move rule count
        //b->enPassSquares.push_back(64);
        b->enPassSquares[currentDepth+1] =64;
        //b->fiftyMoveRuleHalfMoves.push_back(0);
        b->fiftyMoveRuleHalfMoves[currentDepth+1] = 0;

        if(capturedPiece == R){
            if(squareTo == 0){
                castleRights &= ~(1ULL);
            }
            if(squareTo == 7){
                castleRights &= ~(1ULL<<1);
            }
        }
        if(capturedPiece ==r){
            if(squareTo == 56){
                castleRights &= ~(1ULL<<2);
            }
            if(squareTo == 63){
                castleRights &= ~(1ULL<<3);
            }
        }

        if(isEnPass){

            int enPassCapSquare = (squareTo + (8 + ((!side) * -16)));
            unsigned long long enPassBit = 1ULL << enPassCapSquare;
            b->bitboards[movedPiece] |= squareToBit;
            //b->bitboards[friendlyIndex] |= squareToBit;

            b->bitboards[capturedPiece] ^= enPassBit;
            //b->bitboards[enemyIndex] ^= enPassBit;
           //position^= t->zobristPieces[capturedPiece][enPassCapSquare];
           // position^= t->zobristPieces[movedPiece][squareTo];
        }
        if(isPromotion){

            b->bitboards[capturedPiece] ^= squareToBit;
            //b->bitboards[enemyIndex] ^= squareToBit;
            b->bitboards[promotedPiece] |= squareToBit;
            //b->bitboards[friendlyIndex] |= squareToBit;
            //position^= t->zobristPieces[capturedPiece][squareTo];
            //position^= t->zobristPieces[promotedPiece][squareTo];
        }
        //normal captures
        if(!isPromotion && !isEnPass) {
            b->bitboards[movedPiece] |= squareToBit;
            //b->bitboards[friendlyIndex] |= squareToBit;
            b->bitboards[capturedPiece] ^= squareToBit;
            //b->bitboards[enemyIndex] ^= squareToBit;
            //position^= t->zobristPieces[capturedPiece][squareTo];
            //position^= t->zobristPieces[movedPiece][squareTo];
        }

    }
    //non capture moves
    else{


        if(isDoubleP){
            int enPass = squareTo + (8 + ((!side) * -16));
            //b->enPassSquares.push_back(enPass);
            b->enPassSquares[currentDepth+1] = enPass;
            b->bitboards[movedPiece] |= squareToBit;
            //b->bitboards[friendlyIndex] |= squareToBit;

            //position^= t->zobristPieces[movedPiece][squareTo];

        }
        else{
            //b->enPassSquares.push_back(64);
            b->enPassSquares[currentDepth+1] = 64;

            if(isCastle){

                b->bitboards[movedPiece] |= squareToBit;
                //b->bitboards[friendlyIndex] |= squareToBit;
                //position^= t->zobristPieces[movedPiece][squareTo];
                int squareDifference = squareTo - squareFrom;
                int rook = R+side;

                //queenside castle
                if(squareDifference ==-2){
                    int castleSquare1 = squareTo-2;
                    unsigned long long cs1Bit = 1ULL << castleSquare1;
                    b->bitboards[rook] ^= cs1Bit;
                    //b->bitboards[friendlyIndex] ^= cs1Bit;
                    //position^= t->zobristPieces[rook][castleSquare1];
                    int castleSquare2 = squareFrom -1;
                    unsigned long long cs2Bit = 1ULL << castleSquare2;
                    b->bitboards[rook] |= cs2Bit;
                    //b->bitboards[friendlyIndex] |= cs2Bit;
                    //position^= t->zobristPieces[rook][castleSquare2];
                }
                    //kingside castle
                else{
                    int castleSquare1 = squareTo+1;
                    unsigned long long cs1Bit = 1ULL << castleSquare1;
                    b->bitboards[rook] ^= cs1Bit;
                    //b->bitboards[friendlyIndex] ^= cs1Bit;
                    //position^= t->zobristPieces[rook][castleSquare1];
                    int castleSquare2 = squareFrom+1;
                    unsigned long long cs2Bit = 1ULL << castleSquare2;
                    b->bitboards[rook] |= cs2Bit;
                    //b->bitboards[friendlyIndex] |= cs2Bit;
                    //position^= t->zobristPieces[rook][castleSquare2];
                }

            }

            if(isPromotion){

                //unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
                b->bitboards[promotedPiece] |= squareToBit;
                //b->bitboards[friendlyIndex] |= squareToBit;
                //position^= t->zobristPieces[promotedPiece][squareTo];
            }

                //normal moves
            if(!isPromotion && !isCastle){

                b->bitboards[movedPiece] |=  squareToBit;
                //b->bitboards[friendlyIndex] |=  squareToBit;
                //position^= t->zobristPieces[movedPiece][squareTo];
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

    bool newSide = !b->sideToMove;
    b->sideToMove = newSide;
    //b->castleRights.push_back(castleRights);
    b->castleRights[currentDepth+1] = castleRights;

    //position^= t->zobristCastles[castleRights];
    //position^= t->zobristEnPass[b->enPassSquares.back()];
    //position ^= t->zobristBlackTurn;

    //b->currentPosition = position;

   // b->moveHistory.push_back(position);


    if(checkIfInCheck(side,b,t)){
        unmakeMove(m,b,t);
        return false;
    }
    else return true;
}





/* unmakeMove-
 * given a side, move, and board
 * updates the board to reflect the move being taken back
 */

void unmakeMove(unsigned long long m, Board* b,LookupLibrary* t){

    //unsigned long long position = b->currentPosition;

    //int squareFrom = m.squareFrom;
    //int squareTo = m.squareTo;
    //int movedPiece = m.movedPiece;
    //int promotedPiece = m.promotedTo;
    //int capturedPiece = m.capturedPiece;

    //int squareFrom = m.squareFrom;
    unsigned int squareFrom = m & 0b111111;
    //int squareTo = m.squareTo;
    unsigned int squareTo = (m & (0b111111 << 6)) >> 6;
    //int movedPiece = m.movedPiece;
    unsigned int movedPiece = (m & (0b1111 << 17)) >>17;
    //int capturedPiece = m.capturedPiece;
    unsigned int capturedPiece = (m & (0b1111 << 21)) >>21;
    //unsigned int promotedPiece = m.promotedTo;
    unsigned int promotedPiece = (m & (0b1111 << 25)) >> 25;

    unsigned long long isDoubleP = (m & (0b1 <<12));
    unsigned long long isCapture = (m & (0b1 << 13));
    unsigned long long isPromotion = (m& (0b1 <<14));
    unsigned long long isEnPass = (m& (0b1 <<15));
    unsigned long long isCastle = (m&(0b1 <<16));

    //position^= t->zobristEnPass[b->enPassSquares.back()];
    //position^= t->zobristCastles[b->castleRights.back()];



    bool revertSide = !b->sideToMove;

    //int enemyIndex = 0 + !revertSide;
    //int friendlyIndex = 0 + revertSide;
    //unsigned long long movedPieceBitboard = b->bitboards[movedPiece];

    unsigned long long squareFromBit = 1ULL << squareFrom;
    b->bitboards[movedPiece] |= squareFromBit;
    //position^= t->zobristPieces[movedPiece][squareFrom];
    //b->bitboards[friendlyIndex] |= squareFromBit;

    unsigned long long squareToBit = 1ULL << squareTo;
    if(isCapture){

        if(isEnPass){

            int enPassCapSquare = (squareTo + (8 + ((!revertSide) * -16)));
            unsigned long long enPassBit = 1ULL << enPassCapSquare;
            b->bitboards[movedPiece] ^= squareToBit;
            //b->bitboards[friendlyIndex] ^= squareToBit;
            //position^= t->zobristPieces[movedPiece][squareTo];
            b->bitboards[capturedPiece] |= enPassBit;
            //b->bitboards[enemyIndex] |= enPassBit;
            //position^= t->zobristPieces[capturedPiece][enPassCapSquare];

        }
        else{
            if(isPromotion){

                b->bitboards[capturedPiece] |= squareToBit;
                //b->bitboards[enemyIndex] |= squareToBit;
                //position^= t->zobristPieces[capturedPiece][squareTo];
                b->bitboards[promotedPiece] ^= squareToBit;
                //b->bitboards[friendlyIndex] ^= squareToBit;
                //position^= t->zobristPieces[promotedPiece][squareTo];
            }
                //normal captures
            else{

                b->bitboards[movedPiece] ^= squareToBit;
                //b->bitboards[friendlyIndex] ^= squareToBit;
                //position^= t->zobristPieces[movedPiece][squareTo];
                b->bitboards[capturedPiece] |= squareToBit;
                //b->bitboards[enemyIndex] |= squareToBit;
                //position^= t->zobristPieces[capturedPiece][squareTo];
            }
        }


    }
        //non capture moves
    else{

        if(isCastle){

            b->bitboards[movedPiece] ^= squareToBit;
            //b->bitboards[friendlyIndex] ^= squareToBit;
            //position^= t->zobristPieces[movedPiece][squareTo];
            int squareDifference = squareTo - squareFrom;
            int rook = R+revertSide;

            //queenside castle
            if(squareDifference ==-2){

                int castleSquare1 = squareTo-2;
                unsigned long long cs1Bit = 1ULL << castleSquare1;
                b->bitboards[rook] |= cs1Bit;
                //b->bitboards[friendlyIndex] |= cs1Bit;
                //position^= t->zobristPieces[rook][castleSquare1];
                int castleSquare2 = squareFrom-1;
                unsigned long long cs2Bit = 1ULL << castleSquare2;
                b->bitboards[rook] ^= cs2Bit;
               // b->bitboards[friendlyIndex] ^= cs2Bit;
                //position^= t->zobristPieces[rook][castleSquare2];
            }
                //kingside castle
            else{
                int castleSquare1 = squareTo+1;
                unsigned long long cs1Bit = 1ULL << castleSquare1;
                b->bitboards[rook] |= cs1Bit;
                //b->bitboards[friendlyIndex] |= cs1Bit;
                //position^= t->zobristPieces[rook][castleSquare1];
                int castleSquare2 = squareFrom+1;
                unsigned long long cs2Bit = 1ULL << castleSquare2;
                b->bitboards[rook] ^= cs2Bit;
                //b->bitboards[friendlyIndex] ^= cs2Bit;
                //position^= t->zobristPieces[rook][castleSquare2];
            }
        }else{
            if(isPromotion){

                b->bitboards[promotedPiece] ^= squareToBit;
                //b->bitboards[friendlyIndex] ^= squareToBit;
                //position^= t->zobristPieces[promotedPiece][squareTo];
            }
                //normal moves
            else{

                b->bitboards[movedPiece] ^= squareToBit;
                //b->bitboards[friendlyIndex] ^= squareToBit;
                //position^= t->zobristPieces[movedPiece][squareTo];
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
    b->sideToMove = revertSide;
    //b->enPassSquares.pop_back();
    //b->castleRights.pop_back();
    //position^= t->zobristEnPass[b->enPassSquares.back()];
    //position^= t->zobristCastles[b->castleRights.back()];
    //b->fiftyMoveRuleHalfMoves.pop_back();

    //b->moveHistory.pop_back();

    //position ^= t->zobristBlackTurn;
    //b->currentPosition = position;

}

unsigned long long PerftWrapper(int finishDepth, int printDepth, Board* board, LookupLibrary* t){


    auto start = chrono::high_resolution_clock::now();

    unsigned long long nodes = Perft(finishDepth,printDepth,board,t);

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << "Time: "  << duration.count() << " ms "  << endl;

    return nodes;

}
/*Perft-
 * given a finish depth, board, target library and side
 * counts the number of leaf nodes in move tree the side and going until that depth.
 */
unsigned long long Perft(int finishDepth, int printDepth, Board* board, LookupLibrary* t){

    unsigned long long moveCount =0;

    if(finishDepth == 0){
        return 1ULL;
    }

    int listLength = 0;

    unsigned long long list[256];
    generateAllMoves(board,t,&listLength, list);
    //vector<Move> legals = findLegalMoves(board,ms,t);

    //cout << "dwjjdjw" <<endl;


    for(int i =0; i<listLength; i++){

        bool valid = makeMove(list[i],board,t);

        //cout << "blah" << endl;
        if(valid) {

            board->currentDepth++;
            unsigned long ct = Perft(finishDepth - 1, printDepth, board, t);

            moveCount += ct;
            unmakeMove(list[i], board, t);
            board->currentDepth--;
        }

    }


    return moveCount;


}

