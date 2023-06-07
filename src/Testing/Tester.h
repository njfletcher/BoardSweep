//
// Created by nflet on 5/10/2023.
//

#ifndef BOARDSWEEP_TESTER_H
#define BOARDSWEEP_TESTER_H

#endif //BOARDSWEEP_TESTER_H
#include "../Representation/LookupLibrary.h"

void testAll(LookupLibrary* t);
void testPositionKnown(const char* fen, unsigned int depth, unsigned long long nodes,LookupLibrary* t);