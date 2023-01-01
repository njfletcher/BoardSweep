#include <iostream>
#include "BoardVisualization.h"
#include "GameStart.h"
#include "Representation/Move.h"
#include "MoveGeneration/TargetGeneration.h"
#include "BitUtil.h"

using namespace std;
int main() {

    //Board * board = initializeBoardFromFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b - h8 10 2");
    //displayWholeBoard(board);

    unsigned long long ** pawnAttackLookups = initializePawnAttackLookups();

    unsigned long long* wPawnAttacks = *pawnAttackLookups;
    unsigned long long* bPawnAttacks = *(++pawnAttackLookups);

    unsigned long long * knightMoveLookups = initializeKnightMoveLookups();

    unsigned long long * kingMoveLookups = initializeKingMoveLookups();

    unsigned long long * bishopTargetLookups = initializeBishopTargetLookups();
    unsigned long long * rookTargetLookups = initializeRookTargetLookups();

    unsigned long long * bishopTargetCountLookups = initializeBishopTargetCountLookup(bishopTargetLookups);
    unsigned long long * rookTargetCountLookups = initializeRookTargetCountLookup(rookTargetLookups);



    unsigned long long test = 0ULL;
    int count = 0;
    for(int i = 0; i < 64; i++){

      cout<< rookTargetCountLookups[i] << " ";
      count++;
      if(count>7){
          count = 0;
          cout << endl;
      }


    }


    return 0;

}
