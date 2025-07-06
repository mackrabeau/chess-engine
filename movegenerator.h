#ifndef MOVE_H
#define MOVE_H

#include "types.h"
#include "movetables.h"
#include "board.h"
#include <bitset>

#include <iostream>
#include <string>

using namespace std;

struct Move {
    // worst case, 218 possible moves
    // change this to 40 and then if there are more, copy elements into larger array.

    static const int MAX_MOVES = 218; // max number of moves
    U16 moveList[MAX_MOVES];

    short count = 0; // counts number of legal moves

    void addMove(U16 move){
        if (count < MAX_MOVES) {
            moveList[count] = move;
            count++;
        } else {
            cout << "Move list is full, cannot add more moves. fix later\n";
        }

    }

    void clear() {
        count = 0;
    }

    void displayMoves() const {
        int i = 0;
        while (moveList[i]){
            displayMove(i);
            i++;
        }
    }

    void displayMove(int i) const {
        if (i < 0 || i >= count) return; // invalid index

        U16 move = moveList[i];
        int flags = move >> 12;
        int from = move >> 6 & 0x3f;
        int to = move & 0x3f;

        std::cout << "flags: " << flags << ", from : " << from << ", to : " << to << "\n";
    }

    inline U16 getFlags(int i) const {
        if (i < 0 || i >= count) return 0;

        return moveList[i] & 0xF000; // extract flags from move
    }
    
    U16 getMove(int i) { return moveList[i];}
    int getNumMoves() const { return count; }    // returns number of legal moves
};



class Board; // forward declaration

class MoveGenerator {
private: 

    U64 getBishopAttacks(U64 occupancy, int square);
    U64 getRookAttacks(U64 occupancy, int square);

    void generateKingMovesForSquare(const Board& board, int square, Move& pseudoMoves); 
    void generatePawnMovesForSquare(const Board& board, int square, Move& pseudoMoves); 
    void generateBishopMovesForSquare(const Board& board, int square, Move& pseudoMoves); 
    void generateKnightMovesForSquare(const Board& board, int square, Move& pseudoMoves);
    void generateRookMovesForSquare(const Board& board, int square, Move& pseudoMoves); 
    void generateQueenMovesForSquare(const Board& board, int square, Move& pseudoMoves);

    const MoveTables& tables; // reference to move tables

public:

    MoveGenerator(const MoveTables& tables) : tables(tables) {}

    Move generateAllLegalMoves(const Board& board);
    void generateMoves(enumPiece pieceType, const Board& board, int square, Move& pseudoMoves);    // generates bitmap of all legal moves for that pieces

    void addMovesToStruct(Move& pseudoMoves, const Board& board, int square, U64 moves); 

    U64 attackedBB(const Board& board, U8 enemyColour); // checks if square is attacked by enemy pieces

    U16 readMove(U8 from, U8 to, const Board& board);  // returns move int

    void displayBitboard(U64 bitboard, int square, char symbol) const;

};

#endif // MOVE_H
