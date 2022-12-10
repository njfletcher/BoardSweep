//
// Created by nflet on 12/7/2022.
//

#ifndef BOARDSWEEP_BOARDREPRESENTATION_H
#define BOARDSWEEP_BOARDREPRESENTATION_H

#endif //BOARDSWEEP_BOARDREPRESENTATION_H

class Board{

    //using little endian mapping


public:

    unsigned long long bitboards[14];
    int sideToMove;

    //4th bit: white kingside, 3rd bit: white queenside, 2nd bit:black kingside, 1st bit: black queenside
    //set bit means right to castle
    int castleRights;

    //0-63 mean enPassant right exists on square, 64 means enPassant option does not exist
    int enPassSquare;

    int halfMoveCount;
    int fullMoveCount;

    //using single char enum items so fen string chars can be used to directly index bitboard array
    //Uppercase = white piece, lowercase = black piece
    enum enumPiece
    {
        allWhite,
        allBlack,
        P,
        p,
        N,
        n,
        B,
        b,
        R,
        r,
        Q,
        q,
        K,
        k
    };

    enum enumColor{
        white,
        black
    };


    unsigned long long getBitboardByType(enumPiece piece) const;
    unsigned long long getBitboardByTypeAndColor(enumPiece piece, enumColor c) const;





};
