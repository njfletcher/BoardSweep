//
// Created by nflet on 5/16/2023.
//

#include "Eval.h"
#include "EvalConstants.h"
#include "../MoveGeneration/MoveGeneration.h"
#include "../BitUtil.h"
#include "../BoardVisualization.h"
#include <iostream>
using namespace std;

void countAndScoreBits(int* score, unsigned long long bitboard, int piece, int* pieceCounts, bool sideToMove){

    int count = 0;
    int maxSquare = 63;
    int mirrorFlag = 0;
    if(!sideToMove)mirrorFlag = 1;


    while(bitboard>0){

        int square = (int)popLSB(&bitboard);

        square = ((maxSquare - square) * mirrorFlag) + (square * !mirrorFlag);

        *score += MiddlePieceSquareTables[piece / 2][square];
        count++;
    }
    pieceCounts[piece-2] = count;
    *score += count * PieceValues[piece-2];

}

int evaluatePosition(Board* board, bool sideToMove,TargetLibrary* t){

    int score = 0;
    int pieceCounts[12] = {};

    //start at 2 to skip the team bitboards
    for(int i =2; i<14;i++){

        unsigned long long bitboard = board->bitboards[i];
        countAndScoreBits(&score,bitboard,i,pieceCounts,sideToMove);

    }

    return score;
}

