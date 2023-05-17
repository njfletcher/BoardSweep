//
// Created by nflet on 5/16/2023.
//

#include "Eval.h"
#include "../MoveGeneration/MoveGeneration.h"
#include "../BitUtil.h"
#include "../BoardVisualization.h"
#include <iostream>
using namespace std;

int evaluatePosition(Board* board, bool sideToMove,TargetLibrary* t){

    int score = 0;
    int pieceCounts[12] = {};

    //start at 2 to skip the team bitboards
    for(int i =2; i<14;i++){

        unsigned long long bitboard = board->bitboards[i];

        if(bitboard){
            int pieceCount = (int)countSetBits(bitboard);
            pieceCounts[i-2] = pieceCount;

            //cout << i << ": " << pieceCount<< " ";
            //cout << pieceCount * PieceValues[i-2] << " "<< endl;
            score += pieceCount * PieceValues[i-2];
            cout << score <<endl;

        }


    }





    return score;
}