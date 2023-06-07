#include <iostream>
#include "BoardVisualization.h"
#include "GamePlay.h"
#include "MoveGeneration/TargetGeneration.h"
#include "MoveGeneration/MoveGeneration.h"
#include "Representation/LookupLibrary.h"
#include "Testing/Tester.h"
#include "Search/Search.h"
#include <string.h>
#include <math.h>


using namespace std;
int main(int argc, char** argv) {

    LookupLibrary lookup;
    lookup.pawnSinglePushLookups = initializePawnSinglePushTargetLookups();
    lookup.pawnDoublePushLookups = initializePawnDoublePushTargetLookups();
    lookup.pawnAttackLookups = initializePawnAttackTargetLookups();
    lookup.knightMoveLookups = initializeKnightTargetLookups();
    lookup.kingMoveLookups = initializeKingTargetLookups();
    lookup.bishopTargetLookups = initializeBishopTargetLookups();
    lookup.rookTargetLookups = initializeRookTargetLookups();
    lookup.bishopMagicAttacks = initializeBishopMagicAttackTable(lookup.bishopTargetLookups);
    lookup.rookMagicAttacks = initializeRookMagicAttackTable(lookup.rookTargetLookups);
    initializeZobristArrays(&lookup);

    Board b;

    Board* board = initializeBoardFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0",500,&lookup,&b);

    cout << argv[0] << endl;


    // position [fen] or position start
    // perft 5 1

    while(true){

        string input;

        getline(cin,input);

        int inputLen = 0;
        for(int i =0; input[i] != 0; i++){

            inputLen++;
        }

        //cout << input << endl;
        int commandLength = 0;
        for(int i =0; (input[i] != 0) && (input[i] != ' '); i++){
            commandLength++;

        }

        char* command = new char[commandLength +1];

        for(int i = 0; i <commandLength; i++){

            command[i] = input[i];
        }
        command[commandLength] = 0;
        //cout << command << endl;

        if(strcmp(command, "quit") == 0){

            delete[] command;
            return 0;
        }

        if(strcmp(command, "display") == 0){

            displayWholeBoard(board);
        }

        if(strcmp(command, "perft") == 0){

            int depthLength = 0;
            int depth = 0;
            if(commandLength == inputLen) cout << "enter a depth!" << endl;
            else{

                depthLength = inputLen - (commandLength +1);

                for(int i = depthLength; i>0;i--) {

                    depth += ((int) input[commandLength + 1 + (depthLength - i)] - '0') * pow(10, i-1);

                }
                if(depth > 19) cout<< "exceeded max depth" <<endl;
                else{
                    PerftWrapper(depth,400,board,&lookup);
                }
            }

        }

        if(strcmp(command, "position") == 0){


            if(commandLength == inputLen) cout << "enter a fen string!" << endl;
            int fenLength = inputLen - (commandLength+1);
            char* fen = new char[fenLength];
            for(int i = 0; i< fenLength; i++){

                fen[i] = input[commandLength + 1 + i];
            }

            initializeBoardFromFen(fen,fenLength,&lookup,board);
            delete[] fen;

        }



        delete[] command;

    }


    testAll(&lookup);
    //simGame(&lookup,"8/8/8/8/8/k7/p1K5/8 b - - 0 1");
    //simGame(&lookup,"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    //testPositionUnknown("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",6,&lookup);
    //board->currentDepth = 0;
    //cout << "nodes: " << PerftWrapper(6,400,board,&lookup) << endl;





    return 0;


}

