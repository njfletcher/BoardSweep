//
// Created by nflet on 12/7/2022.
//

#ifndef BOARDSWEEP_BOARDREPRESENTATION_H
#define BOARDSWEEP_BOARDREPRESENTATION_H

#endif //BOARDSWEEP_BOARDREPRESENTATION_H

class Board{

    //using little endian mapping

    unsigned long long bitboards[14];
public:
    enum enumPiece
    {
        allWhite,
        allBlack,
        whitePawn,
        blackPawn,
        whiteKnight,
        blackKnight,
        whiteBishop,
        blackBishop,
        whiteRook,
        blackRook,
        whiteQueen,
        blackQueen,
        whiteKing,
        blackKing
    };

    enum enumColor{
        white,
        black
    };


    unsigned long long getBitboardByType(enumPiece piece) const;
    unsigned long long getBitboardByTypeAndColor(enumPiece piece, enumColor c) const;





};
