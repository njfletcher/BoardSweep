//
// Created by nflet on 5/18/2023.
//

#include "Search.h"
#include "Eval.h"
#include "../GamePlay.h"
#include "../BoardVisualization.h"
#include "../Representation/Board.h"
#include "../Representation/UsefulConstants.h"
#include "../MoveGeneration/MoveGeneration.h"
#include <limits.h>
#include<algorithm>

//white will be maximizer, black will be minimizer
MovePair startAB(int currentDepth, TargetLibrary* t,Board* board, bool side){

    return searchAB(currentDepth,INT_MIN,INT_MAX,t,board,side,Move());

}

MovePair searchAB(int currentDepth, int alpha, int beta, TargetLibrary* t,Board* board,bool side,Move m){

    if(currentDepth == 0) return MovePair(m,evaluatePosition(board,side,t,false,false));


    vector<Move> moves = generateAllMoves(side,board,t);
    vector<Move> legals = findLegalMoves(side,board,moves,t);

    if(legals.size() == 0){
        unsigned long long attackMask = getAttackMask(!side,board->bitboards,t);

        unsigned long long kingBit = board->bitboards[K+side];

        if(attackMask & kingBit){
            return MovePair(m,evaluatePosition(board,side,t,true,false));
        }
        else return MovePair(m,evaluatePosition(board,side,t,false,true));

    }

    //minimizing player
    if(side == black){

        int minEval = INT_MAX;

        MovePair minMove(Move(),minEval);

        for(Move m : legals){

            makeMove(side,m,board);
            int score = searchAB(currentDepth-1,alpha,beta,t,board,!side,m).evalScore;
            unmakeMove(side,m,board);

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

            makeMove(side,m,board);
            int score = searchAB(currentDepth-1,alpha,beta,t,board,!side,m).evalScore;
            unmakeMove(side,m,board);

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



