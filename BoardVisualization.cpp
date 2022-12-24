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

void displayWholeBoard(Board * board){

    cout<< board->sideToMove << " " << board->enPassSquare << " " << board->castleRights << " " << board->halfMoveCount << " " << board-> fullMoveCount;
    cout << endl;
    cout << "----------------All Pieces----------------------" << endl;
    displayBitboard(board->bitboards[allBlack] || board->bitboards[allWhite] );

    cout << "----------------White Pieces----------------------" << endl;
    displayBitboard(board->bitboards[allWhite] );

    cout << "----------------Black Pieces----------------------" << endl;
    displayBitboard(board->bitboards[allBlack] );

    cout << "----------------White Pawns----------------------" << endl;
    displayBitboard(board->bitboards[P] );

    cout << "----------------Black Pawns----------------------" << endl;
    displayBitboard(board->bitboards[p] );

    cout << "----------------White Bishops----------------------" << endl;
    displayBitboard(board->bitboards[B] );

    cout << "----------------Black Bishops----------------------" << endl;
    displayBitboard(board->bitboards[b] );
    cout << "----------------White Knights----------------------" << endl;
    displayBitboard(board->bitboards[N] );

    cout << "----------------Black Knights----------------------" << endl;
    displayBitboard(board->bitboards[n] );

    cout << "----------------White Rooks----------------------" << endl;
    displayBitboard(board->bitboards[R] );

    cout << "----------------Black Rooks----------------------" << endl;
    displayBitboard(board->bitboards[r] );

    cout << "----------------White Queens----------------------" << endl;
    displayBitboard(board->bitboards[Q] );

    cout << "----------------Black Queens----------------------" << endl;
    displayBitboard(board->bitboards[q] );

    cout << "----------------White King----------------------" << endl;
    displayBitboard(board->bitboards[K] );

    cout << "----------------Black King----------------------" << endl;
    displayBitboard(board->bitboards[k] );

    cout << endl;


}