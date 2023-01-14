//
// Created by nflet on 12/9/2022.
//

#include "GameStart.h"
#include <math.h>
#include <iostream>
using namespace std;


Board*  initializeBoardFromFen(const char fen[]){
    //"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

    Board  * board = new Board;

    for(int i = 0; i<14;i++){
        board->bitboards[i] =0;
    }
    unsigned int currentSquare = 56;
    unsigned int charCount =0;
    unsigned int numSquaresVisited =0;


    //load the squares
    while(numSquaresVisited <64){

        char occupancy = fen[charCount];


        if(occupancy == '/'){
            currentSquare-=16;
        }
        else{
            //ascii value of 9 is 57
            if(occupancy <57){
                currentSquare+= (occupancy - '0');
                numSquaresVisited +=(occupancy - '0');

            }
            else{


                switch(occupancy){

                    case 'P':
                        board->bitboards[P] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'p':
                        board->bitboards[p] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'N':
                        board->bitboards[N] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'n':
                        board->bitboards[n] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'B':
                        board->bitboards[B] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'b':
                        board->bitboards[b] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'R':
                        board->bitboards[R] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'r':
                        board->bitboards[r] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'Q':
                        board->bitboards[Q] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'q':
                        board->bitboards[q] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'K':
                        board->bitboards[K] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;
                    case 'k':
                        board->bitboards[k] |= 1ULL << currentSquare;
                        currentSquare++;
                        numSquaresVisited++;
                        break;

                }

            }
        }

        charCount++;

    }
    board->bitboards[allWhite] = board->bitboards[P] | board->bitboards[B] | board->bitboards[N] |
            board->bitboards[R] | board->bitboards[Q] | board->bitboards[K];

    board->bitboards[allBlack] = board->bitboards[p] | board->bitboards[b] | board->bitboards[n] |
                                 board->bitboards[r] | board->bitboards[q] | board->bitboards[k];



    charCount++;
    if(fen[charCount] == 'w'){
        board->sideToMove = white;
    }
    else board->sideToMove = black;
    charCount+=2;

    unsigned int tempRights = 0;
    while(!(fen[charCount] == ' ')){
        char castleChar = fen[charCount];
        switch(castleChar){

            case 'K':
                tempRights |= 1L <<3;
                break;
            case 'Q':
                tempRights |= 1L<<2;
                break;
            case 'k':
                tempRights |= 1L <<1;
                break;
            case 'q':
                tempRights |= 1L;
                break;
            default:
                break;
        }
        board->castleRights.push_back(tempRights);
        charCount++;
    }
    charCount++;

    if(fen[charCount] == '-'){
        board->enPassSquares.push_back(64);
        charCount++;
    }
    else{
        char file = fen[charCount];
        charCount++;
        char rank = fen[charCount];

        board->enPassSquares.push_back(((rank - '0')*8)+ (file - '`'-9));
        charCount++;

    }


    charCount++;

    int decimalCountHalf =0;
    int halfMoveCount =0;
    while(!(fen[charCount] == ' ')){
        decimalCountHalf++;
        charCount++;
    }

    charCount-=decimalCountHalf;


    for(int i =decimalCountHalf-1; i>=0;i--){

        //cout << pow(10,i)* (fen[charCount]-'0')<<endl;
        halfMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
    }

    board->fiftyMoveRuleHalfMoves.push_back(halfMoveCount);
    charCount++;

    int decimalCountFull =0;
    int fullMoveCount =0;
    while(!(fen[charCount] == '\0')){
        decimalCountFull++;
        charCount++;
    }

    charCount-=decimalCountFull;


    for(int i =decimalCountFull-1; i >=0;i--){

        fullMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
    }

    board->fullMoveCount = fullMoveCount;


    return board;

}