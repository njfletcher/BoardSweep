#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/MoveGeneration.h"

int main() {

    //Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    //displayWholeBoard(board);

    displayBitboard(generateBlackPawnSingleTarget((unsigned long long)0xFF<<8,~((unsigned long long)0xFF<<8)));

    return 0;

}
