#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <cstdint>
#include <string>

typedef uint64_t U64;
typedef uint16_t U16;
typedef uint8_t U8;


// could maybe combine this with enumPiece
// enum PieceType { Pawn, Knight, Bishop, Rook, Queen, King, Empty };
// enum ColourType { White, Black };

enum enumPiece {
    nWhite,     // all white pieces
    nBlack,     // all black pieces
    nPawns,     
    nKnights,   
    nBishops,   
    nRooks,     
    nQueens,    
    nKings,
    nEmpty,      
};

class Board {
public:
    U64 pieceBB[8];

    // this will store game info, change name later
    // colour's turn
    // castling rights
    // num moves w/out pawn move or check

    U16 gameInfo;

    Board(const std::string& fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); 
    Board(const Board& other);
    Board(U64 otherPieceBB[8], const U16& otherGameInfo);

    U64 getAllPieces() const {return pieceBB[nWhite] | pieceBB[nBlack];};
    // U64 getPieceSet(enumPiece pt, enumPiece ct) const {return pieceBB[pieceCode(pt)] & pieceBB[colourCode(ct)];};

    U64 getWhitePawns() const {return pieceBB[nPawns] & pieceBB[nWhite];}
    U64 getWhiteKnights() const {return pieceBB[nKnights] & pieceBB[nWhite];}
    U64 getWhiteBishops() const {return pieceBB[nBishops] & pieceBB[nWhite];}
    U64 getWhiteRooks() const {return pieceBB[nRooks] & pieceBB[nWhite];}
    U64 getWhiteQueens() const {return pieceBB[nQueens] & pieceBB[nWhite];}
    U64 getWhiteKing() const {return pieceBB[nKings] & pieceBB[nWhite];}

    U64 getBlackPawns() const {return pieceBB[nPawns] & pieceBB[nBlack];}
    U64 getBlackKnights() const {return pieceBB[nKnights] & pieceBB[nBlack];}
    U64 getBlackBishops() const {return pieceBB[nBishops] & pieceBB[nBlack];}
    U64 getBlackRooks() const {return pieceBB[nRooks] & pieceBB[nBlack];}
    U64 getBlackQueens() const {return pieceBB[nQueens] & pieceBB[nBlack];}
    U64 getBlackKing() const {return pieceBB[nKings] & pieceBB[nBlack];}

    // U64 getPawns(ColourType ct) const {return pieceBB[nPawns] & pieceBB[colourCode(ct)];}
    // U64 getKnights(ColourType ct) const {return pieceBB[nKnights] & pieceBB[colourCode(ct)];}
    // U64 getBishops(ColourType ct) const {return pieceBB[nBishops] & pieceBB[colourCode(ct)];}
    // U64 getRooks(ColourType ct) const {return pieceBB[nRooks] & pieceBB[colourCode(ct)];}
    // U64 getQueens(ColourType ct) const {return pieceBB[nQueens] & pieceBB[colourCode(ct)];}
    // U64 getKing(ColourType ct) const {return pieceBB[nKings] & pieceBB[colourCode(ct)];}

    void displayBoard() const;
    void displayGameInfo() const;  // add this
    enumPiece coloursTurn() const;
    void coloursTurnToString() const;

    enumPiece getPieceType(int square) const;
    enumPiece getColourType(int square) const;

    U8 friendlyColour() const { return gameInfo & 1ULL; }
    U8 enemyColour() const { return !(gameInfo & 1ULL); }

    char pieceToChar(int square) const;

    Board makeMove(const U16& move);

    U8 getFrom(const U16& move) const{ return (move >> 6) & 0x3F; }
    U8 getTo(const U16& move) const { return move & 0x3F; }
    U8 getFlags(const U16& move) { return (move >> 10) & 0x3F; }


private:
    void clearBoard();
    void loadPiece(char piece, int square);

    int colourCode(enumPiece ct) const;
    int pieceCode(enumPiece pt) const;
};

#endif // BOARD_H
