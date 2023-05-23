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

void countAndScoreBits(int* score, unsigned long long bitboard, int piece, int* pieceCounts){

    int count = 0;
    int mirrorFlag = 0;
    int scoreSign = -1;
    //white piece int values will always be even
    if((piece % 2) ==0){
        mirrorFlag = 1;
        scoreSign = 1;
    }

    while(bitboard>0){

        int square = (int)popLSB(&bitboard);

        square = ((square ^ 63) * mirrorFlag) + (square * !mirrorFlag);

        //since both colors for a given piece map to the same pst table
        int adjustedPiece = (piece/2)-1;

        *score += scoreSign * MiddlePieceSquareTables[adjustedPiece][square];
        count++;
    }
    pieceCounts[piece-2] = count;
    *score += count * PieceValues[piece-2];

}

int evaluatePosition(Board* board, LookupLibrary* t, bool isCheckMate, bool isDraw,int depth){

    bool sideToMove= board->sideToMove;
    int score = 0;
    int pieceCounts[12] = {};

    int scoreFlag = -1;
    if(sideToMove) scoreFlag = 1;


    if(isCheckMate)return (depth* 10000 * scoreFlag);

    //start at 2 to skip the team bitboards
    for(int i =2; i<14;i++){

        unsigned long long bitboard = board->bitboards[i];
        countAndScoreBits(&score,bitboard,i,pieceCounts);

    }


    if(isDraw){

        return depth * score * -1;

    }

    return score;
}

