//
// Created by nflet on 5/18/2023.
//

#include "Search.h"
#include "Eval.h"
#include "../BitUtil.h"
#include "../GamePlay.h"
#include "../BoardVisualization.h"
#include "../Representation/Board.h"
#include "../Representation/UsefulConstants.h"
#include "../MoveGeneration/MoveGeneration.h"
#include <limits.h>
#include<algorithm>




//white will be maximizer, black will be minimizer
MovePair startAB(int currentDepth, LookupLibrary* t,Board* board){

    unsigned long long invalidMove = 1ULL<<63;
    return searchAB(currentDepth,INT_MIN,INT_MAX,t,board,invalidMove);

}

MovePair searchAB(int currentDepth, int alpha, int beta, LookupLibrary* t,Board* board,unsigned long long move){

    bool side = board->sideToMove;

    unsigned long long list[256];
    int listLength = 0;
    generateAllMoves(board,t,&listLength,list);

    int repetitionCount =1;
    unsigned long long currentPos = board->currentPosition;
    for(int i =0;i<board->moveHistory.size()-1;i++){
        if(board->moveHistory[i]== currentPos) repetitionCount++;
        if(repetitionCount >=3)return MovePair(move,evaluatePosition(board,t,false,true,currentDepth,listLength));
    }
    /*if(listLength == 0){
        unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

        unsigned long long kingBit = board->bitboards[K+side];

        if(attackMask & kingBit){
            return MovePair(move,evaluatePosition(board,t,true,false,currentDepth,0));
        }
        else return MovePair(move,evaluatePosition(board,t,false,true,currentDepth,0));

    }
     */

    if(board->fiftyMoveRuleHalfMoves[board->currentDepth] >=100)return MovePair(move,evaluatePosition(board,t,false,true,currentDepth,listLength));
    if(currentDepth == 0) return MovePair(move,evaluatePosition(board,t,false,false,currentDepth,listLength));


    bool hasMoves = false;
    //minimizing player
    if(side == black){

        int minEval = INT_MAX;

        MovePair minMove(0,minEval);

        for(int i =0; i <listLength; i++){

            unsigned long long move = list[i];
            bool valid = makeMove(move,board,t);
            if(valid) {

                board->currentDepth++;
                hasMoves = true;
                int score = searchAB(currentDepth - 1, alpha, beta, t, board, move).evalScore;
                unmakeMove(move, board, t);
                board->currentDepth--;

                if (score < minEval) {
                    minMove.m = move;
                    minMove.evalScore = score;
                }

                minEval = std::min(minEval, score);

                beta = std::min(beta, score);

                if (beta <= alpha) return minMove;
            }
        }
        if(!hasMoves){
            //unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

            unsigned long long kingBit = board->bitboards[K+side];

            unsigned int square = popLSB(&kingBit);
            if(checkIfSquareAttacked(side,board,t,square)){
                return MovePair(move,evaluatePosition(board,t,true,false,currentDepth,0));
            }
            else return MovePair(move,evaluatePosition(board,t,false,true,currentDepth,0));
        }

        return minMove;
    }
    //maximizing player
    else{
        int maxEval = INT_MIN;
        MovePair maxMove(0,maxEval);

        for(int i =0; i <listLength; i++){

            unsigned long long move = list[i];

            bool valid = makeMove(move,board,t);
            if(valid) {

                board->currentDepth++;
                hasMoves = true;
                int score = searchAB(currentDepth - 1, alpha, beta, t, board, move).evalScore;
                unmakeMove(move, board, t);
                board->currentDepth--;

                if(score > maxEval){
                    maxMove.m = move;
                    maxMove.evalScore = score;
                }
                maxEval = std::max(maxEval,score);
                alpha = std::max(alpha,score);

                if(beta <= alpha) return maxMove;
            }


        }
        if(!hasMoves){
            //unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

            unsigned long long kingBit = board->bitboards[K+side];

            unsigned int square = popLSB(&kingBit);
            if(checkIfSquareAttacked(side,board,t,square)){
                return MovePair(move,evaluatePosition(board,t,true,false,currentDepth,0));
            }
            else return MovePair(move,evaluatePosition(board,t,false,true,currentDepth,0));
        }

        return maxMove;

    }

}


void initializeZobristArrays(LookupLibrary* t){

    t->zobristBlackTurn = generateRandomBitboard();

    unsigned long long* castles = t->zobristCastles;
    for(int i =0;i<16;i++){

       *castles = generateRandomBitboard();
       castles++;
    }

    unsigned long long* enPasses = t->zobristEnPass;
    for(int i =0; i<65;i++){
        *enPasses = generateRandomBitboard();
        enPasses++;
    }

    unsigned long long* pieces = *t->zobristPieces;
    for(int i =0; i<12 * 64;i++){
        *pieces = generateRandomBitboard();
        pieces++;
    }

}



