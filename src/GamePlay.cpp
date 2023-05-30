//
// Created by nflet on 12/9/2022.
//

#include "GamePlay.h"
#include <math.h>
#include <iostream>
#include "BoardVisualization.h"
#include "BitUtil.h"
#include "Representation/Board.h"
#include "Representation/UsefulConstants.h"
#include "Representation/Move.h"
#include "Search/Search.h"
#include "MoveGeneration/MoveGeneration.h"
using namespace std;

void initializeZobristPosition(Board* board, LookupLibrary* t){

    unsigned long long position;

    for(int piece = 2; piece<15;piece++){

        unsigned long long bitboard = board->bitboards[piece];
        while(bitboard){

            int square = popLSB(&bitboard);
            position ^= t->zobristPieces[piece][square];

        }
    }

    //use the castle right 4 bit value(0-16) as a direct index
    //position ^= t->zobristCastles[board->castleRights.back()];
    position ^= t->zobristCastles[board->castleRights[0]];

    //returns 0-63 for valid, 64 if not available. use enPassSquare as direct index
    //int enPassSquare = board->enPassSquares.back();
    int enPassSquare = board->enPassSquares[0];
    position ^= t->zobristEnPass[enPassSquare];

    if(board->sideToMove == black) position ^= t->zobristBlackTurn;

    board->currentPosition = position;
}



Board*  initializeBoardFromFen(const char fen[],LookupLibrary* t){
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

            case 'k':
                tempRights |= 1L <<3;
                break;
            case 'q':
                tempRights |= 1L<<2;
                break;
            case 'K':
                tempRights |= 1L <<1;
                break;
            case 'Q':
                tempRights |= 1L;
                break;
            default:
                break;
        }
        //board->castleRights.push_back(tempRights);
        board->castleRights[0]= tempRights;
        charCount++;
    }
    charCount++;

    if(fen[charCount] == '-'){
        //board->enPassSquares.push_back(64);
        board->enPassSquares[0] = 64;
        charCount++;
    }
    else{
        char file = fen[charCount];
        charCount++;
        char rank = fen[charCount];

        //board->enPassSquares.push_back(((rank - '0')*8)+ (file - '`'-9));
        board->enPassSquares[0] = ((rank - '0')*8)+ (file - '`'-9);
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

    //board->fiftyMoveRuleHalfMoves.push_back(halfMoveCount);
    board->fiftyMoveRuleHalfMoves[0] = halfMoveCount;
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


    initializeZobristPosition(board,t);
    board->moveHistory.push_back(board->currentPosition);
    return board;

}

void simGame(LookupLibrary* t,const char fen[]){

    Board* board = initializeBoardFromFen(fen,t);

    displayWholeBoard(board);

    while(true){

        MovePair movep = startAB(5,t,board);

        Move m = movep.m;
        int eval = movep.evalScore;


        if(!m.isValid) break;
        makeMove(m,board,t);
        //board->sideToMove = !board->sideToMove;

        displayWholeBoard(board);
        cout << "Eval: " << eval << endl;



    }
}