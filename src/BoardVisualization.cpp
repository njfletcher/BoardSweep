//
// Created by nflet on 12/9/2022.
//

#include "BoardVisualization.h"

#include <iostream>

using namespace std;

void displayBitboard(unsigned long long binary){

    unsigned long long util = 1;
    int rankCount = 1;

    while(rankCount<9){

        cout <<endl;
        for(int square = 64 - (rankCount * 8); square < 64 - (rankCount * 8)+8; square++){

            cout << " ";
            cout <<( (binary & ( util << square ))>> square);

        }

        rankCount++;

    }
    cout << endl;

}

void displayChessboard(Board *board){

    unsigned long long* bitboards = board->bitboards;
    unsigned long long util = 1;
    int rankCount = 1;

    while(rankCount<9){

        cout <<endl;
        for(int square = 64 - (rankCount * 8); square < 64 - (rankCount * 8)+8; square++){

            unsigned long long setBit = 1ULL<<square;
            bool occupied = false;
            //find out which piece is getting captured
            for(int piece = 2; piece <14;piece++){

                if(bitboards[piece] & setBit){
                    cout <<" ";
                    cout << PieceChars[piece-2];
                    occupied = true;
                }

            }
            if(!occupied){
                cout <<" ";
                cout << "-";
            }

        }

        rankCount++;

    }
    cout << endl;

}

void displayWholeBoard(Board * board){

    cout<< "Side to move: " <<board->sideToMove <<endl;
    cout<< "EnPassant Square: " << board->enPassSquares.back()<<endl;
    cout<< "Castle Rights: " << board->castleRights.back()<< endl;
    cout <<"Half move count: " << board->fiftyMoveRuleHalfMoves.back() << endl;
    cout << "Full move count: " << board-> fullMoveCount << endl;
    cout << endl;
    displayChessboard(board);
    cout << endl;


}