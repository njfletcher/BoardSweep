#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"

int main() {

    //Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    //displayWholeBoard(board);

    unsigned long long ** pawnMoveLookups = initializePawnMoveLookups();

    unsigned long long* wPawnMoves = *pawnMoveLookups;
    unsigned long long* bPawnMoves = *(++pawnMoveLookups);

    unsigned long long ** pawnAttackLookups = initializePawnAttackLookups();

    unsigned long long* wPawnAttacks = *pawnAttackLookups;
    unsigned long long* bPawnAttacks = *(++pawnAttackLookups);

    unsigned long long * knightMoveLookups = initializeKnightMoveLookups();

    unsigned long long * kingMoveLookups = initializeKingMoveLookups();

    for(int i = 0; i < 64; i++){

        displayBitboard(generateQueenTargetEmptyBoard(i));

    }



    return 0;

}
