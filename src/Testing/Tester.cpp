//
// Created by nflet on 5/10/2023.
//

#include "Tester.h"
#include "../MoveGeneration/MoveGeneration.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include "../GamePlay.h"
using namespace std;

void testAll(TargetLibrary* t){

    ifstream file ("C:\\Users\\nflet\\Desktop\\ChessEngine\\src\\Testing\\perft.txt");

    string currLine;

    if(file.is_open()){

        while(file){

            getline (file, currLine);
            if(currLine[0] == '#'){

                cout << "Testing: ";
                //get fen
                int count = 0;
                for(int i =1; currLine[i] != ','; i++) count++;

                char fen[count+1];
                for(int i = 0; i<count;i++){
                    fen[i] = currLine[i+1];
                }
                fen[count] = 0;

                cout << "fen " << fen;

                //get depth
                int count2 = 0;
                unsigned int depth = 0;
                for(int i =count + 2; currLine[i] != ','; i++) count2++;

                for(int i =0; i<count2;i++){

                    depth += pow(10,count2-i-1)* (currLine[count + 2 + i]-'0');

                }

                cout << " with depth: " << depth;

                //get nodes
                int count3 = 0;
                unsigned int nodes = 0;
                for(int i =count + 2+count2 + 1; currLine[i] != ','; i++) count3++;
                for(int i =0; i<count3;i++){

                    nodes += pow(10,count3-i-1)* (currLine[count + 2 + count2 +1 + i]-'0');
                }

                cout << " expected nodes: " << nodes << endl;
                testPosition(fen,depth,nodes,t);
            }
        }

    }
    else cout << "could not open file" <<endl;

    file.close();
}

void testPosition(const char fen[], unsigned int depth, unsigned long long nodes,TargetLibrary* t){


    Board* board = initializeBoardFromFen(fen);
    unsigned long long nodeCount = Perft(depth,400,board,t,board->sideToMove);
    //cout << nodeCount <<endl;
    if(nodeCount != nodes) cout <<"!!!!!!!!!!!!!!!mismatch with position: expected "<< nodes << " got " << nodeCount << "!!!!!!!!!!!!!!!!!!!!" << endl;
    else cout << "position correct expected : " <<  nodes << " got: " << nodeCount <<endl;


    delete board;

}
