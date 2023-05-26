//
// Created by nflet on 1/5/2023.
//
#include "MoveGeneration.h"
#include "../BitUtil.h"
#include "../Representation/UsefulConstants.h"
#include "../Representation/LookupLibrary.h"
#include "TargetGeneration.h"
#include <iostream>
#include "../BoardVisualization.h"
#include <vector>
using namespace std;

//white = 0 black =1;
/*getAttackMask-
 * given a side, bitboard array, and target library
 * returns a bitboard with set bits representing all possible squares
 * that side can pseudo-legally attack
 */
unsigned long long getAttackMask(bool side,unsigned long long* bitboards, LookupLibrary* t){

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

void generateAllCaptures(Board* board, LookupLibrary* t, vector<Move>* moves){


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

            Move m(fromSquare,toSquare,0,1,0,0,0,Q+side,capturedPiece,0);
            moves->push_back(m);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,R+side,capturedPiece,0);
            moves->push_back(m);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,B+side,capturedPiece,0);
            moves->push_back(m);
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

            Move m(fromSquare,toSquare,0,1,0,0,0,N+side,capturedPiece,0);
            moves->push_back(m);
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

        Move m(kingSquare,toSquare,0,1,0,0,0,K+side,capturedPiece,0);
        moves->push_back(m);
    }

    unsigned long long pawns = bitboards[P+side];

    while(pawns){

        int fromSquare = popLSB(&pawns);

        unsigned long long pawnAttackTargets = t->pawnAttackLookups[side][fromSquare];
        unsigned long long pawnNormalCaptures =  pawnAttackTargets & enemyPieces;

        int enPassSquare = board->enPassSquares.back();

        //enpass square of 64 means enpass is not available
        if(enPassSquare !=64){
            unsigned long long enPassBit = 1ULL << enPassSquare;

            if(pawnAttackTargets & enPassBit){
                Move m3(fromSquare,enPassSquare,0,1,0,1,0,P+side,P+!side,0);
                moves->push_back(m3);
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
                Move m(fromSquare,toSquare,0,1,1,0,0,P+side,capturedPiece,N+side);
                moves->push_back(m);

                Move m1(fromSquare,toSquare,0,1,1,0,0,P+side,capturedPiece,B+side);
                moves->push_back(m1);

                Move m2(fromSquare,toSquare,0,1,1,0,0,P+side,capturedPiece,R+side);
                moves->push_back(m2);

                Move m3(fromSquare,toSquare,0,1,1,0,0,P+side,capturedPiece,Q+side);
                moves->push_back(m3);

            }
            else{
                Move m(fromSquare,toSquare,0,1,0,0,0,P+side,capturedPiece,0);
                moves->push_back(m);
            }



        }



    }

}
/* generateAllMoves-
 * given a side, bitboard array, and target library
 * returns vector of all psuedo-legal moves(attack,normal, and special)
 * that a side can make
 */
vector<Move> generateAllMoves(Board* board, LookupLibrary* t){

    bool side = board->sideToMove;
    vector<Move> moveList;

    unsigned long long* bitboards = board->bitboards;


    generateAllCaptures(board,t,&moveList);

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

    }

    unsigned long long knights = bitboards[N+side];
    while(knights){
        int fromSquare = popLSB(&knights);

        unsigned long long knightAll = t->knightMoveLookups[fromSquare];
        unsigned long long knightQuietMoves = knightAll & ~allPieces;
       // unsigned long long knightCaptureMoves = knightAll & enemyPieces;
        while(knightQuietMoves){

            int toSquare = popLSB(&knightQuietMoves);

            Move m(fromSquare,toSquare,0,0,0,0,0,N+side,0,0);
            moveList.push_back(m);
        }

    }


    unsigned long long bishops = bitboards[B+side];
    while(bishops){
        int fromSquare = popLSB(&bishops);

        unsigned long long bishopAll = getBishopTargetFromBlockers(fromSquare,allPieces  & t->bishopTargetLookups[fromSquare],t->bishopMagicAttacks);
        unsigned long long bishopQuietMoves = bishopAll  & ~allPieces;
        //unsigned long long bishopCaptureMoves = bishopAll  & enemyPieces;

        while(bishopQuietMoves){

            int toSquare = popLSB(&bishopQuietMoves);
            Move m(fromSquare,toSquare,0,0,0,0,0,B+side,0,0);
            moveList.push_back(m);
        }


    }


    unsigned long long rooks = bitboards[R+side];
    while(rooks){
        int fromSquare = popLSB(&rooks);

        unsigned long long rookAll = getRookTargetFromBlockers(fromSquare,allPieces  & t->rookTargetLookups[fromSquare],t->rookMagicAttacks);
        unsigned long long rookQuietMoves = rookAll  & ~allPieces;
        //unsigned long long rookCaptureMoves = rookAll & enemyPieces;

        while(rookQuietMoves){
            int toSquare = popLSB(&rookQuietMoves);

            Move m(fromSquare,toSquare,0,0,0,0,0,R+side,0,0);
            moveList.push_back(m);
        }

    }

    unsigned long long queens = bitboards[Q+side];
    while(queens){
        int fromSquare = popLSB(&queens);

        unsigned long long queenAll = getQueenTargetFromBlockers(fromSquare,allPieces,t);
        unsigned long long queenQuietMoves = queenAll & ~allPieces;
        //unsigned long long queenCaptureMoves = queenAll & enemyPieces;

        while(queenQuietMoves){

            int toSquare = popLSB(&queenQuietMoves);
            Move m(fromSquare,toSquare,0,0,0,0,0,Q+side,0,0);
            moveList.push_back(m);
        }

    }

    unsigned long long king = bitboards[K+side];

    //assumption is that there will always have to be a single king on the board for each side
    int kingSquare = popLSB(&king);

    unsigned long long kingAll = t->kingMoveLookups[kingSquare];
    unsigned long long kingQuietMoves =  kingAll & ~allPieces;
    //unsigned long long kingCaptureMoves = kingAll & enemyPieces;

    while(kingQuietMoves){

        int toSquare = popLSB(&kingQuietMoves);
        Move m(kingSquare,toSquare,0,0,0,0,0,K+side,0,0);
        moveList.push_back(m);

    }


    unsigned int castleRights = board->castleRights.back();

    bool sideHasQueenSideCastle = castleRights & (1ULL<<(0+(side * 2))) && 1;
    bool sideHasKingSideCastle = castleRights & (1ULL<<(1+(side * 2))) && 1;

    unsigned long long enemyAttack = getAttackMask(!side,bitboards,t);

    if(sideHasKingSideCastle){


        int castleIndex = side * 2;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        if(!((enemyAttack & CastleCheckSquares[castleIndex]) || ( CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL<<kingSquare)))) ){
            Move m(kingSquare,kingSquare+2,0,0,0,0,1,K+side,0,0);
            moveList.push_back(m);

        }



    }
    if(sideHasQueenSideCastle){

        int castleIndex = side * 2 + 1;
        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
        if(!((enemyAttack & CastleCheckSquares[castleIndex]) || ( CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL<<kingSquare)))) ){
            Move m(kingSquare,kingSquare-2,0,0,0,0,1,K+side,0,0);
            moveList.push_back(m);

        }

    }


    return moveList;

}

/* makeMove-
 * given a side, move, and board
 * updates the board to reflect the move being made
 */

void makeMove(Move m, Board* b, LookupLibrary* t){

    unsigned long long position = b->currentPosition;

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;
    int movedPiece = m.movedPiece;
    int capturedPiece = m.capturedPiece;
    int promotedPiece = m.promotedTo;

    bool side = b->sideToMove;

    int enemyIndex = 0 + !side;
    int friendlyIndex = 0 + side;

    unsigned long long squareFromBit = 1ULL << squareFrom;
    b->bitboards[movedPiece] ^= squareFromBit;
    b->bitboards[friendlyIndex] ^= squareFromBit;

    position^= t->zobristPieces[movedPiece][squareFrom];

    position^= t->zobristEnPass[b->enPassSquares.back()];

    unsigned int castleRights = b->castleRights.back();
    position^= t->zobristCastles[castleRights];

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

    unsigned long long squareToBit = 1ULL << squareTo;

    if(m.capture){

        //capture makes enPassant unavailable and resets 50 move rule count
        b->enPassSquares.push_back(64);
        b->fiftyMoveRuleHalfMoves.push_back(0);

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

        if(m.enPassant){

            int enPassCapSquare = (squareTo + (8 + ((!side) * -16)));
            unsigned long long enPassBit = 1ULL << enPassCapSquare;
            b->bitboards[movedPiece] |= squareToBit;
            b->bitboards[friendlyIndex] |= squareToBit;

            b->bitboards[capturedPiece] ^= enPassBit;
            b->bitboards[enemyIndex] ^= enPassBit;
            position^= t->zobristPieces[capturedPiece][enPassCapSquare];
            position^= t->zobristPieces[movedPiece][squareTo];
        }
        if(m.promotion){

            b->bitboards[capturedPiece] ^= squareToBit;
            b->bitboards[enemyIndex] ^= squareToBit;
            b->bitboards[promotedPiece] |= squareToBit;
            b->bitboards[friendlyIndex] |= squareToBit;
            position^= t->zobristPieces[capturedPiece][squareTo];
            position^= t->zobristPieces[promotedPiece][squareTo];
        }
        //normal captures
        if(!m.promotion && !m.enPassant) {
            b->bitboards[movedPiece] |= squareToBit;
            b->bitboards[friendlyIndex] |= squareToBit;
            b->bitboards[capturedPiece] ^= squareToBit;
            b->bitboards[enemyIndex] ^= squareToBit;
            position^= t->zobristPieces[capturedPiece][squareTo];
            position^= t->zobristPieces[movedPiece][squareTo];
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
            b->bitboards[movedPiece] |= squareToBit;
            b->bitboards[friendlyIndex] |= squareToBit;

            position^= t->zobristPieces[movedPiece][squareTo];

        }
        else{
            b->enPassSquares.push_back(64);

            if(m.castle){

                b->bitboards[movedPiece] |= squareToBit;
                b->bitboards[friendlyIndex] |= squareToBit;
                position^= t->zobristPieces[movedPiece][squareTo];
                int squareDifference = squareTo - squareFrom;
                int rook = R+side;

                //queenside castle
                if(squareDifference ==-2){
                    int castleSquare1 = squareTo-2;
                    unsigned long long cs1Bit = 1ULL << castleSquare1;
                    b->bitboards[rook] ^= cs1Bit;
                    b->bitboards[friendlyIndex] ^= cs1Bit;
                    position^= t->zobristPieces[rook][castleSquare1];
                    int castleSquare2 = squareFrom -1;
                    unsigned long long cs2Bit = 1ULL << castleSquare2;
                    b->bitboards[rook] |= cs2Bit;
                    b->bitboards[friendlyIndex] |= cs2Bit;
                    position^= t->zobristPieces[rook][castleSquare2];
                }
                    //kingside castle
                else{
                    int castleSquare1 = squareTo+1;
                    unsigned long long cs1Bit = 1ULL << castleSquare1;
                    b->bitboards[rook] ^= cs1Bit;
                    b->bitboards[friendlyIndex] ^= cs1Bit;
                    position^= t->zobristPieces[rook][castleSquare1];
                    int castleSquare2 = squareFrom+1;
                    unsigned long long cs2Bit = 1ULL << castleSquare2;
                    b->bitboards[rook] |= cs2Bit;
                    b->bitboards[friendlyIndex] |= cs2Bit;
                    position^= t->zobristPieces[rook][castleSquare2];
                }

            }

            if(m.promotion){

                //unsigned long long promotedPieceBitboard = b->bitboards[m.promotedTo];
                b->bitboards[promotedPiece] |= squareToBit;
                b->bitboards[friendlyIndex] |= squareToBit;
                position^= t->zobristPieces[promotedPiece][squareTo];
            }

                //normal moves
            if(!m.promotion && !m.castle){

                b->bitboards[movedPiece] |=  squareToBit;
                b->bitboards[friendlyIndex] |=  squareToBit;
                position^= t->zobristPieces[movedPiece][squareTo];
            }



        }

    }

    bool newSide = !b->sideToMove;
    b->sideToMove = newSide;
    b->castleRights.push_back(castleRights);

    position^= t->zobristCastles[castleRights];
    position^= t->zobristEnPass[b->enPassSquares.back()];
    position ^= t->zobristBlackTurn;

    b->currentPosition = position;

    b->moveHistory.push_back(position);


}

/* findLegalMoves-
 * given a side, board, pseudo-legal set of moves, and target library
 * finds all legal moves from the set of pseudo legal moves.
 */
vector<Move> findLegalMoves(Board* board, vector<Move> allMoves, LookupLibrary* t){

    vector<Move> legalMoveList;

    bool side = board->sideToMove;

    for(int i = 0; i<allMoves.size(); i++){

        Move currMove = allMoves[i];
        makeMove(currMove,board,t);

        unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

        unsigned long long kingBit = board->bitboards[K+side];

        if(!(attackMask & kingBit)){
            legalMoveList.push_back(currMove);
        }

        unmakeMove(currMove,board,t);

    }

    return legalMoveList;
}

/* unmakeMove-
 * given a side, move, and board
 * updates the board to reflect the move being taken back
 */

void unmakeMove(Move m, Board* b,LookupLibrary* t){

    unsigned long long position = b->currentPosition;

    int squareFrom = m.squareFrom;
    int squareTo = m.squareTo;
    int movedPiece = m.movedPiece;
    int promotedPiece = m.promotedTo;
    int capturedPiece = m.capturedPiece;

    position^= t->zobristEnPass[b->enPassSquares.back()];
    position^= t->zobristCastles[b->castleRights.back()];



    bool revertSide = !b->sideToMove;

    int enemyIndex = 0 + !revertSide;
    int friendlyIndex = 0 + revertSide;
    unsigned long long movedPieceBitboard = b->bitboards[movedPiece];

    unsigned long long squareFromBit = 1ULL << squareFrom;
    b->bitboards[movedPiece] |= squareFromBit;
    position^= t->zobristPieces[movedPiece][squareFrom];
    b->bitboards[friendlyIndex] |= squareFromBit;

    unsigned long long squareToBit = 1ULL << squareTo;
    if(m.capture){

        unsigned long long capturedPieceBitboard = b->bitboards[m.capturedPiece];

        if(m.enPassant){

            int enPassCapSquare = (squareTo + (8 + ((!revertSide) * -16)));
            unsigned long long enPassBit = 1ULL << enPassCapSquare;
            b->bitboards[movedPiece] ^= squareToBit;
            b->bitboards[friendlyIndex] ^= squareToBit;
            position^= t->zobristPieces[movedPiece][squareTo];
            b->bitboards[capturedPiece] |= enPassBit;
            b->bitboards[enemyIndex] |= enPassBit;
            position^= t->zobristPieces[capturedPiece][enPassCapSquare];

        }
        else{
            if(m.promotion){

                b->bitboards[capturedPiece] |= squareToBit;
                b->bitboards[enemyIndex] |= squareToBit;
                position^= t->zobristPieces[capturedPiece][squareTo];
                b->bitboards[promotedPiece] ^= squareToBit;
                b->bitboards[friendlyIndex] ^= squareToBit;
                position^= t->zobristPieces[promotedPiece][squareTo];
            }
                //normal captures
            else{

                b->bitboards[movedPiece] ^= squareToBit;
                b->bitboards[friendlyIndex] ^= squareToBit;
                position^= t->zobristPieces[movedPiece][squareTo];
                b->bitboards[capturedPiece] |= squareToBit;
                b->bitboards[enemyIndex] |= squareToBit;
                position^= t->zobristPieces[capturedPiece][squareTo];
            }
        }


    }
        //non capture moves
    else{

        if(m.castle){

            b->bitboards[movedPiece] ^= squareToBit;
            b->bitboards[friendlyIndex] ^= squareToBit;
            position^= t->zobristPieces[movedPiece][squareTo];
            int squareDifference = squareTo - squareFrom;
            int rook = R+revertSide;

            //queenside castle
            if(squareDifference ==-2){

                int castleSquare1 = squareTo-2;
                unsigned long long cs1Bit = 1ULL << castleSquare1;
                b->bitboards[rook] |= cs1Bit;
                b->bitboards[friendlyIndex] |= cs1Bit;
                position^= t->zobristPieces[rook][castleSquare1];
                int castleSquare2 = squareFrom-1;
                unsigned long long cs2Bit = 1ULL << castleSquare2;
                b->bitboards[rook] ^= cs2Bit;
                b->bitboards[friendlyIndex] ^= cs2Bit;
                position^= t->zobristPieces[rook][castleSquare2];
            }
                //kingside castle
            else{
                int castleSquare1 = squareTo+1;
                unsigned long long cs1Bit = 1ULL << castleSquare1;
                b->bitboards[rook] |= cs1Bit;
                b->bitboards[friendlyIndex] |= cs1Bit;
                position^= t->zobristPieces[rook][castleSquare1];
                int castleSquare2 = squareFrom+1;
                unsigned long long cs2Bit = 1ULL << castleSquare2;
                b->bitboards[rook] ^= cs2Bit;
                b->bitboards[friendlyIndex] ^= cs2Bit;
                position^= t->zobristPieces[rook][castleSquare2];
            }
        }else{
            if(m.promotion){

                b->bitboards[promotedPiece] ^= squareToBit;
                b->bitboards[friendlyIndex] ^= squareToBit;
                position^= t->zobristPieces[promotedPiece][squareTo];
            }
                //normal moves
            else{

                b->bitboards[movedPiece] ^= squareToBit;
                b->bitboards[friendlyIndex] ^= squareToBit;
                position^= t->zobristPieces[movedPiece][squareTo];
            }
        }

    }

    b->sideToMove = revertSide;
    b->enPassSquares.pop_back();
    b->castleRights.pop_back();
    position^= t->zobristEnPass[b->enPassSquares.back()];
    position^= t->zobristCastles[b->castleRights.back()];
    b->fiftyMoveRuleHalfMoves.pop_back();

    b->moveHistory.pop_back();

    position ^= t->zobristBlackTurn;
    b->currentPosition = position;

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

    bool side = board->sideToMove;

    vector<Move> ms = generateAllMoves(board,t);
    vector<Move> legals = findLegalMoves(board,ms,t);

    for(Move m : legals){

        makeMove(m,board,t);

        unsigned long ct = Perft(finishDepth - 1,printDepth, board,t);
        if(printDepth == finishDepth){
            m.toString();
            m.toUCI();
            cout << ct << " nodes" << endl;
        }
        moveCount += ct;
        unmakeMove(m,board,t);

    }

    return moveCount;


}
void generateMovesCertainDepth(int depth,Board* board, LookupLibrary* t){

    if(depth ==0){
        return;
    }

    bool side = board->sideToMove;
    vector<Move> ms = generateAllMoves(board,t);
    vector<Move> legals = findLegalMoves(board,ms,t);
    displayWholeBoard(board);

    for(Move m : legals){


            cout << "BEFORE==========================================" << endl;
            displayWholeBoard(board);

            makeMove(m, board,t);
            cout << "MAKE============================================" << endl;
            m.toString();
            m.toUCI();
            displayWholeBoard(board);

            generateMovesCertainDepth(depth - 1, board, t);

            unmakeMove( m, board,t);
            cout << "UNMAKE==========================================" << endl;
            displayWholeBoard(board);

    }

    return;
}
