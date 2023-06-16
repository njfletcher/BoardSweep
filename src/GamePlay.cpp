//
// Created by nflet on 12/9/2022.
//

#include "GamePlay.h"
#include <math.h>
#include <iostream>
#include <cstring>
#include "BoardVisualization.h"
#include "BitUtil.h"
#include "Representation/Board.h"
#include "Representation/UsefulConstants.h"
#include "Representation/Move.h"
#include "Search/Search.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/TargetGeneration.h"
using namespace std;

void initializeZobristPosition(Board* board, LookupLibrary* t){

    /*
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
     */
}



Board*  initializeBoardFromFen(const char fen[],int fenLength,LookupLibrary* t, Board* board){
    //"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"


    int length = fenLength;

    for(int i = 0; i<14;i++){
        board->bitboards[i] =0;
    }
    unsigned int currentSquare = 56;
    unsigned int charCount =0;
    unsigned int numSquaresVisited =0;


    //load the squares
    while(numSquaresVisited <64){

        char occupancy = fen[charCount];

        if(charCount > length) return board;

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
    if(charCount > length)return board;
    if(fen[charCount] == 'w'){
        board->sideToMove = white;
    }
    else board->sideToMove = black;
    charCount+=2;
    if(charCount > length)return board;

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
    if(charCount > length)return board;

    if(fen[charCount] == '-'){
        //board->enPassSquares.push_back(64);
        board->enPassSquares[0] = 64;
        charCount++;
        if(charCount > length)return board;
    }
    else{
        char file = fen[charCount];
        charCount++;
        if(charCount > length)return board;
        char rank = fen[charCount];

        //board->enPassSquares.push_back(((rank - '0')*8)+ (file - '`'-9));
        board->enPassSquares[0] = ((rank - '0')*8)+ (file - '`'-9);
        charCount++;
        if(charCount > length)return board;

    }


    charCount++;
    if(charCount > length)return board;
    int decimalCountHalf =0;
    int halfMoveCount =0;
    while(!(fen[charCount] == ' ')){
        decimalCountHalf++;
        charCount++;
        if(charCount > length)return board;
    }

    charCount-=decimalCountHalf;
    if(charCount > length)return board;

    for(int i =decimalCountHalf-1; i>=0;i--){

        //cout << pow(10,i)* (fen[charCount]-'0')<<endl;
        halfMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
        if(charCount > length)return board;
    }

    //board->fiftyMoveRuleHalfMoves.push_back(halfMoveCount);
    board->fiftyMoveRuleHalfMoves[0] = halfMoveCount;
    charCount++;
    if(charCount > length)return board;

    int decimalCountFull =0;
    int fullMoveCount =0;
    while(!(fen[charCount] == '\0')){
        decimalCountFull++;
        charCount++;
        if(charCount > length)return board;
    }

    charCount-=decimalCountFull;

    if(charCount > length)return board;

    for(int i =decimalCountFull-1; i >=0;i--){

        fullMoveCount += pow(10,i)* (fen[charCount]-'0');
        charCount++;
        if(charCount > length)return board;
    }

    board->fullMoveCount = fullMoveCount;


    initializeZobristPosition(board,t);
    board->moveHistory.push_back(board->currentPosition);
    return board;

}

void simGame(LookupLibrary* t,const char fen[],int fenLength){

    Board b;
    Board* board = initializeBoardFromFen(fen,fenLength,t,&b);

    displayWholeBoard(board);

    while(true){

        MovePair movep = startAB(6,t,board);

        unsigned long long m = movep.m;
        int eval = movep.evalScore;


        if(m & (1ULL<<63)) break;
        makeMove(m,board,t,false);
        //board->sideToMove = !board->sideToMove;

        displayWholeBoard(board);
        cout << "Eval: " << eval << endl;



    }
}

void playGame(LookupLibrary* t, Board* b, int sideFirst){

    displayWholeBoard(b);
    bool playerTurn = false;

    if(sideFirst == 1)playerTurn = true;


    while(true) {

        if (!playerTurn) {
            MovePair movep = startAB(6, t, b);

            unsigned long long m = movep.m;
            if (m & (1ULL << 63)) break;
            makeMove(m, b, t, false);
            displayWholeBoard(b);
            playerTurn = !playerTurn;
        } else {
            cout << "make a move: " << endl;
            bool isValid = false;
            unsigned long long move;
            while (!isValid) {

                bool side = b->sideToMove;

                string input;

                getline(cin, input);

                int inputLen = 0;
                for (int i = 0; input[i] != 0; i++) {

                    inputLen++;
                }


                unsigned long long blackPieces = b->bitboards[1];
                unsigned long long whitePieces = b->bitboards[0];

                unsigned long long allPieces = blackPieces | whitePieces;

                if (input == "O-O") {

                    bool sideHasKingSideCastle = (b->castleRights[0] & (1ULL << (1 + (side * 2)))) != 0;

                    if (sideHasKingSideCastle) {
                        unsigned int kingSquare1 = 4 + 56 * side;
                        unsigned int kingSquare2 = 5 + 56 * side;

                        int castleIndex = side * 2;


                        if (!((checkIfSquareAttacked(side, b, t, kingSquare1) ||
                               (checkIfSquareAttacked(side, b, t, kingSquare2))) ||
                              (CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL << kingSquare1))))) {
                            move = packageMove(kingSquare1, kingSquare1 + 2, false, false, false,
                                               false, true, K + side, 0, 0);
                            isValid = true;

                        } else {
                            cout << "invalid castle" << endl;
                            continue;
                        }
                    } else {
                        cout << "invalid castle" << endl;
                        continue;
                    }

                }
                if (input == "O-O-O") {

                    bool sideHasQueenSideCastle = (b->castleRights[0] & (1ULL << (0 + (side * 2)))) != 0;

                    if (sideHasQueenSideCastle) {

                        unsigned int queenSquare1 = 4 + 56 * side;
                        unsigned int queenSquare2 = 3 + 56 * side;

                        int castleIndex = side * 2 + 1;
                        //cant castle if a piece is in between king and rook, or if opponent is attacking relevant castle squares.
                        if (!(((checkIfSquareAttacked(side, b, t, queenSquare1) ||
                                (checkIfSquareAttacked(side, b, t, queenSquare2))) ||
                               (CastleBlockSquares[castleIndex] & (allPieces & ~(1ULL << queenSquare1)))))) {
                            move = packageMove(queenSquare1, queenSquare1 - 2, false, false, false, false, true,
                                               K + side, 0, 0);
                            isValid = true;

                        } else {
                            cout << "invalid castle" << endl;
                            continue;
                        }

                    } else {
                        cout << "invalid castle" << endl;
                        continue;
                    }

                }

                if ((input != "O-O") && (input != "O-O-O")) {

                    if ((inputLen < 4) || (inputLen > 5)) {
                        cout << "invalid move length" << endl;
                        continue;
                    }
                    char fromFile = input[0];
                    char fromRank = input[1];
                    char toFile = input[2];
                    char toRank = input[3];

                    unsigned int squareFrom = ((fromRank - '0') * 8) + (fromFile - '`' - 9);
                    unsigned int squareTo = ((toRank - '0') * 8) + (toFile - '`' - 9);

                    //cout<< squareFrom << " " << squareTo << endl;

                    if ((squareFrom > 63) || (squareTo > 63)) {
                        cout << "invalid move square" << endl;
                        continue;
                    }

                    int movedPiece = -1;
                    unsigned long long fromBit = 1ULL << squareFrom;
                    for (int piece = (2 + (side)); piece < 14; piece += 2) {
                        if (b->bitboards[piece] & fromBit) {
                            movedPiece = piece;
                            break;
                        }


                    }
                    if (movedPiece == -1) {
                        cout << "invalid moved piece" << endl;
                        continue;
                    }

                    unsigned long long targets = ~0;
                    if (movedPiece == B + side) {
                        targets = getBishopTargetFromBlockers(squareFrom,
                                                              allPieces & t->bishopTargetLookups[squareFrom],
                                                              t->bishopMagicAttacks);

                    }
                    if (movedPiece == R + side) {
                        targets = getRookTargetFromBlockers(squareFrom, allPieces & t->rookTargetLookups[squareFrom],
                                                            t->rookMagicAttacks);
                    }
                    if (movedPiece == N + side) {
                        targets = t->knightMoveLookups[squareFrom];
                    }
                    if (movedPiece == Q + side) {
                        targets = getQueenTargetFromBlockers(squareFrom, allPieces, t);
                    }
                    if (movedPiece == K + side) {
                        targets = t->kingMoveLookups[squareFrom];
                    }

                    if (!(targets & (1ULL << squareTo))) {
                        cout << "invalid landing spot" << endl;
                        continue;
                    }
                    int nextPiece = -1;
                    unsigned long long toBit = 1ULL << squareTo;
                    for (int piece = (2 + (side)); piece < 14; piece += 2) {
                        if (b->bitboards[piece] & toBit) {
                            nextPiece = piece;
                            break;
                        }


                    }

                    if (nextPiece != -1) {
                        cout << "invalid landing spot" << endl;
                        continue;
                    }

                    for (int piece = (2 + (!side)); piece < 14; piece += 2) {
                        if (b->bitboards[piece] & toBit) {
                            nextPiece = piece;
                            break;
                        }


                    }

                    unsigned long long promRank = RankMasks[7 + (-7 * side)];
                    //capture
                    if (nextPiece != -1) {

                        if (movedPiece == P) {
                            targets = t->pawnAttackLookups[0][squareFrom];
                        }
                        if (movedPiece == p) {
                            targets = t->pawnAttackLookups[1][squareFrom];
                        }

                        if (!(targets & (1ULL << squareTo))) {
                            cout << "invalid landing spot" << endl;
                            continue;
                        }

                        bool isPromotion = false;

                        if ((toBit & promRank) && (movedPiece == P + side)) {

                            if (inputLen == 5) {
                                isPromotion = true;
                                bool validProm = false;
                                char promotedTo = input[4];
                                if (promotedTo == Q + side) {
                                    move = packageMove(squareFrom, squareTo, false, true, true, false, false, P + side,
                                                       nextPiece, Q + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == R + side) {
                                    move = packageMove(squareFrom, squareTo, false, true, true, false, false, P + side,
                                                       nextPiece, R + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == N + side) {
                                    move = packageMove(squareFrom, squareTo, false, true, true, false, false, P + side,
                                                       nextPiece, N + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == B + side) {
                                    move = packageMove(squareFrom, squareTo, false, true, true, false, false, P + side,
                                                       nextPiece, B + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (!validProm) {
                                    cout << "invalid move" << endl;
                                    continue;
                                }
                            } else {
                                cout << "invalid move" << endl;
                                continue;
                            }
                        }

                        if (!isPromotion) {
                            move = packageMove(squareFrom, squareTo, false, true, false, false, false, movedPiece,
                                               nextPiece, 0);
                            isValid = true;
                        }

                    }
                        //non capture or could be enpassant
                    else {

                        bool isEnPassant = false;
                        bool isPromotion = false;
                        bool isDoubleP = false;

                        if (squareTo == b->enPassSquares[0] && (movedPiece == P + side)) {

                            if (movedPiece == P) {
                                targets = t->pawnAttackLookups[0][squareFrom];
                            }
                            if (movedPiece == p) {
                                targets = t->pawnAttackLookups[1][squareFrom];
                            }

                            if (!(targets & (1ULL << squareTo))) {
                                cout << "invalid landing enpass spot" << endl;
                                continue;
                            }

                            move = packageMove(squareFrom, squareTo, false, true, false, true, false, P + side,
                                               P + !side, 0);
                            isValid = true;
                            isEnPassant = true;
                        }
                        if ((toBit & promRank) && (movedPiece == P + side)) {

                            if (movedPiece == P) {
                                targets = t->pawnSinglePushLookups[0][squareFrom];
                            }
                            if (movedPiece == p) {
                                targets = t->pawnSinglePushLookups[1][squareFrom];
                            }

                            if (!(targets & (1ULL << squareTo))) {
                                cout << "invalid landing prom  spot" << endl;
                                continue;
                            }

                            if (inputLen == 5) {
                                isPromotion = true;
                                bool validProm = false;
                                char promotedTo = input[4];
                                if (promotedTo == Q + side) {
                                    move = packageMove(squareFrom, squareTo, false, false, true, false, false, P + side,
                                                       0, Q + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == R + side) {
                                    move = packageMove(squareFrom, squareTo, false, false, true, false, false, P + side,
                                                       0, R + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == N + side) {
                                    move = packageMove(squareFrom, squareTo, false, false, true, false, false, P + side,
                                                       0, N + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (promotedTo == B + side) {
                                    move = packageMove(squareFrom, squareTo, false, false, true, false, false, P + side,
                                                       0, B + side);
                                    isValid = true;
                                    validProm = true;
                                }
                                if (!validProm) {
                                    cout << "invalid move" << endl;
                                    continue;
                                }
                            } else {
                                cout << "invalid move" << endl;
                                continue;
                            }
                        }

                        if ((squareTo == (squareFrom + 16 - 32 * side)) && (movedPiece == P + side)) {

                            if (movedPiece == P) {
                                targets = t->pawnDoublePushLookups[0][squareFrom];
                            }
                            if (movedPiece == p) {
                                targets = t->pawnDoublePushLookups[1][squareFrom];
                            }

                            if (!(targets & (1ULL << squareTo))) {
                                cout << "invalid landing double spot" << endl;
                                continue;
                            }
                            move = packageMove(squareFrom, squareTo, true, false, false, false, false, P + side, 0, 0);
                            isValid = true;
                            isDoubleP = true;

                        }

                        if (!isDoubleP && !isPromotion && !isEnPassant) {

                            if (movedPiece == P) {
                                targets = t->pawnSinglePushLookups[0][squareFrom];
                            }
                            if (movedPiece == p) {
                                targets = t->pawnSinglePushLookups[1][squareFrom];
                            }

                            if (!(targets & (1ULL << squareTo))) {
                                cout << "invalid landing single spot" << endl;
                                continue;
                            }
                            move = packageMove(squareFrom, squareTo, false, false, false, false, false, movedPiece, 0,
                                               0);
                            isValid = true;
                        }

                    }


                }

                if(isValid){
                    bool valid = makeMove(move, b, t, false);
                    if(!valid){
                        cout << "move results in check" << endl;
                        unmakeMove(move,b,t);
                        continue;
                    }
                    displayWholeBoard(b);
                    playerTurn = !playerTurn;

                }

            }



        }

    }




}

