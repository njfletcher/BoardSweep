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

int quiescenceSearch(int alpha, int beta,Board* board,LookupLibrary* t, bool isCheckM, bool isDraw, int depth){


    int standPat = evaluatePosition(board,t,isCheckM,isDraw,depth);

    if( standPat >= beta )
        return beta;
    if( alpha < standPat )
        alpha = standPat;

    bool side = board->sideToMove;

    vector<Move> moveList;
    generateAllCaptures(board,t,&moveList);
    vector<Move> legals = findLegalMoves(board,moveList,t);
    for(Move m : legals){

        makeMove(m,board);
        int score = quiescenceSearch(alpha,beta,board,t,isCheckM,isDraw,depth);
        unmakeMove(m,board);

        if(score >= beta){
            return beta;
        }
        if(score > alpha){
            alpha = score;
        }

    }
    return alpha;

}



//white will be maximizer, black will be minimizer
MovePair startAB(int currentDepth, LookupLibrary* t,Board* board){

    return searchAB(currentDepth,INT_MIN,INT_MAX,t,board,Move());

}

MovePair searchAB(int currentDepth, int alpha, int beta, LookupLibrary* t,Board* board,Move m){

    if(currentDepth == 0) return MovePair(m,evaluatePosition(board,t,false,false,currentDepth));


    bool side = board->sideToMove;
    vector<Move> moves = generateAllMoves(board,t);
    vector<Move> legals = findLegalMoves(board,moves,t);

    if(legals.size() == 0){
        unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

        unsigned long long kingBit = board->bitboards[K+side];

        if(attackMask & kingBit){
            return MovePair(m,evaluatePosition(board,t,true,false,currentDepth));
        }
        else return MovePair(m,evaluatePosition(board,t,false,true,currentDepth));

    }
    if(board->fiftyMoveRuleHalfMoves.back() >=100)return MovePair(m,evaluatePosition(board,t,false,true,currentDepth));

    //minimizing player
    if(side == black){

        int minEval = INT_MAX;

        MovePair minMove(Move(),minEval);

        for(Move m : legals){

            makeMove(m,board);
            int score = searchAB(currentDepth-1,alpha,beta,t,board,m).evalScore;
            unmakeMove(m,board);

            if(score < minEval){
                minMove.m = m;
                minMove.evalScore = score;
            }

            minEval = std::min(minEval,score);

            beta = std::min(beta,score);

            if(beta <= alpha) return minMove;
        }

        return minMove;
    }
    //maximizing player
    else{
        int maxEval = INT_MIN;
        MovePair maxMove(Move(),maxEval);

        for(Move m : legals){

            makeMove(m,board);
            int score = searchAB(currentDepth-1,alpha,beta,t,board,m).evalScore;
            unmakeMove(m,board);

            if(score > maxEval){
                maxMove.m = m;
                maxMove.evalScore = score;
            }
            maxEval = std::max(maxEval,score);
            alpha = std::max(alpha,score);

            if(beta <= alpha) return maxMove;
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
    for(int i =0; i<8;i++){
        *enPasses = generateRandomBitboard();
        enPasses++;
    }

    unsigned long long* pieces = *t->zobristPieces;
    for(int i =0; i<12 * 64;i++){
        *pieces = generateRandomBitboard();
        pieces++;
    }

}



