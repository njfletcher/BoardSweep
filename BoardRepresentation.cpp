//
// Created by nflet on 12/7/2022.
//

#include "BoardRepresentation.h"

unsigned long long Board::getBitboardByType(enumPiece piece)const{
    return bitboards[piece];
}
unsigned long long Board::getBitboardByTypeAndColor(enumPiece piece, enumColor c) const{
    return bitboards[piece + c];
}

