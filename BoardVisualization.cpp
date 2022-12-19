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