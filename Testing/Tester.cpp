//
// Created by nflet on 5/10/2023.
//

#include "Tester.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void testAll(){

    ifstream file ("Testing/perft.txt");

    string currLine;

    if(file.is_open()){

        while(file){


            getline (file, currLine);
            cout << currLine <<endl;
        }



    }
    else cout << "could not open file" <<endl;

    file.close();
}

void testPosition(const char fen[]){





}