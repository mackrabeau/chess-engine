#ifndef BOARD_H
#define BOARD_H

#include "movegenerator.h"
#include "types.h"

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

class Board {
public:
    U64 pieceBB[8];
    U16 gameInfo;

    Board(const std::string& fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); 
    Board(const Board& other);
    Board(U64 otherPieceBB[8], const U16& otherGameInfo);
    int getEnPassantSquare() const;

    U64 getAllPieces() const {return pieceBB[nWhite] | pieceBB[nBlack];};
    U64 getFriendlyPieces() const {return pieceBB[friendlyColour()];};
    U64 getEnemyPieces() const {return pieceBB[enemyColour()];};

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

    void displayBoard() const;
    void displayGameInfo() const;  // add this
    void coloursTurnToString() const;

    enumPiece getPieceType(int square) const;
    enumPiece getColourType(int square) const;

    U8 friendlyColour() const { return gameInfo & TURN_MASK; }
    U8 enemyColour() const { return !(gameInfo & TURN_MASK); }

    char pieceToChar(int square) const;

    // void applyMove(const U16& move, U64* pieceBBTarget, U16& gameInfoTarget);

    bool isInCheck() const; // check if the current player's king is in check
    bool isInCheck(U8 colour) const; // check if the specified player's king is in check


    static void applyMove(const U16& move, U64* pieceBBTarget, U16& gameInfoTarget, const Board& board);

    // U8 getFrom(const U16& move) const{ return (move >> 6) & 0x3F; }
    // U8 getTo(const U16& move) const { return move & 0x3F; }
    // // U8 getFlags(const U16& move) { return (move >> 10) & 0x3F; }


private:

    void clearBoard();
    void loadPiece(char piece, int square);

    int colourCode(enumPiece ct) const;
    int pieceCode(enumPiece pt) const;
};

#endif // BOARD_H
