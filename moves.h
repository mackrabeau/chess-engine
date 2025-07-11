#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include <cstdint>
#include <iostream>
#include <string>

typedef uint64_t U64;
typedef uint16_t U16;
typedef uint16_t U16;

using namespace std;

const U64 RMagic[64] = {
0x2080020500400f0ULL,
0x28444000400010ULL,
0x20000a1004100014ULL,
0x20010c090202006ULL,
0x8408008200810004ULL,
0x1746000808002ULL,
0x2200098000808201ULL,
0x12c0002080200041ULL,
0x104000208e480804ULL,
0x8084014008281008ULL,
0x4200810910500410ULL,
0x100014481c20400cULL,
0x4014a4040020808ULL,
0x401002001010a4ULL,
0x202000500010001ULL,
0x8112808005810081ULL,
0x40902108802020ULL,
0x42002101008101ULL,
0x459442200810c202ULL,
0x81001103309808ULL,
0x8110000080102ULL,
0x8812806008080404ULL,
0x104020000800101ULL,
0x40a1048000028201ULL,
0x4100ba0000004081ULL,
0x44803a4003400109ULL,
0xa010a00000030443ULL,
0x91021a000100409ULL,
0x4201e8040880a012ULL,
0x22a000440201802ULL,
0x30890a72000204ULL,
0x10411402a0c482ULL,
0x40004841102088ULL,
0x40230000100040ULL,
0x40100010000a0488ULL,
0x1410100200050844ULL,
0x100090808508411ULL,
0x1410040024001142ULL,
0x8840018001214002ULL,
0x410201000098001ULL,
0x8400802120088848ULL,
0x2060080000021004ULL,
0x82101002000d0022ULL,
0x1001101001008241ULL,
0x9040411808040102ULL,
0x600800480009042ULL,
0x1a020000040205ULL,
0x4200404040505199ULL,
0x2020081040080080ULL,
0x40a3002000544108ULL,
0x4501100800148402ULL,
0x81440280100224ULL,
0x88008000000804ULL,
0x8084060000002812ULL,
0x1840201000108312ULL,
0x5080202000000141ULL,
0x1042a180880281ULL,
0x900802900c01040ULL,
0x8205104104120ULL,
0x9004220000440aULL,
0x8029510200708ULL,
0x8008440100404241ULL,
0x2420001111000bdULL,
0x4000882304000041ULL,
};
const U64 BMagic[64] = {
0x100420000431024ULL,
0x280800101073404ULL,
0x42000a00840802ULL,
0xca800c0410c2ULL,
0x81004290941c20ULL,
0x400200450020250ULL,
0x444a019204022084ULL,
0x88610802202109aULL,
0x11210a0800086008ULL,
0x400a08c08802801ULL,
0x1301a0500111c808ULL,
0x1280100480180404ULL,
0x720009020028445ULL,
0x91880a9000010a01ULL,
0x31200940150802b2ULL,
0x5119080c20000602ULL,
0x242400a002448023ULL,
0x4819006001200008ULL,
0x222c10400020090ULL,
0x302008420409004ULL,
0x504200070009045ULL,
0x210071240c02046ULL,
0x1182219000022611ULL,
0x400c50000005801ULL,
0x4004010000113100ULL,
0x2008121604819400ULL,
0xc4a4010000290101ULL,
0x404a000888004802ULL,
0x8820c004105010ULL,
0x28280100908300ULL,
0x4c013189c0320a80ULL,
0x42008080042080ULL,
0x90803000c080840ULL,
0x2180001028220ULL,
0x1084002a040036ULL,
0x212009200401ULL,
0x128110040c84a84ULL,
0x81488020022802ULL,
0x8c0014100181ULL,
0x2222013020082ULL,
0xa00100002382c03ULL,
0x1000280001005c02ULL,
0x84801010000114cULL,
0x480410048000084ULL,
0x21204420080020aULL,
0x2020010000424a10ULL,
0x240041021d500141ULL,
0x420844000280214ULL,
0x29084a280042108ULL,
0x84102a8080a20a49ULL,
0x104204908010212ULL,
0x40a20280081860c1ULL,
0x3044000200121004ULL,
0x1001008807081122ULL,
0x50066c000210811ULL,
0xe3001240f8a106ULL,
0x940c0204030020d4ULL,
0x619204000210826aULL,
0x2010438002b00a2ULL,
0x884042004005802ULL,
0xa90240000006404ULL,
0x500d082244010008ULL,
0x28190d00040014e0ULL,
0x825201600c082444ULL,
};

const int RBits[64] = {
12, 11, 11, 11, 11, 11, 11, 12,
11, 10, 10, 10, 10, 10, 10, 11,
11, 10, 10, 10, 10, 10, 10, 11,
11, 10, 10, 10, 10, 10, 10, 11,
11, 10, 10, 10, 10, 10, 10, 11,
11, 10, 10, 10, 10, 10, 10, 11,
11, 10, 10, 10, 10, 10, 10, 11,
12, 11, 11, 11, 11, 11, 11, 12
};
const int BBits[64] = {
6, 5, 5, 5, 5, 5, 5, 6,
5, 5, 5, 5, 5, 5, 5, 5,
5, 5, 7, 7, 7, 7, 5, 5,
5, 5, 7, 9, 9, 7, 5, 5,
5, 5, 7, 9, 9, 7, 5, 5,
5, 5, 7, 7, 7, 7, 5, 5,
5, 5, 5, 5, 5, 5, 5, 5,
6, 5, 5, 5, 5, 5, 5, 6
};

const U64 EDGE_MASK = 0xff818181818181ffULL;

struct Move {
    // worst case, 218 possible moves
    // change this to 40 and then if there are more, copy elements into larger array.
    // on average save storage
    U16 moveList[218];

    short count;
    
    void addMove(U16 move){
        moveList[count++] = move;
    }

};

struct GMagic {
    U64 mask;  // mask to extract relevant occupancy bits
    U64 magic; // magic number for hashing
    int bits;  // number of bits needed 
};

class MoveGenerator {

private: 
    // 6 pieces :  Pawn, Knight, Bishop, Rook, Queen, King
    Move moveStruct;   

    U64 pawnMovesBB[2][64];
    U64 pawnMovesCapturesBB[2][64];
    // U64 moveBitboards[5][64];
    U64 kingKnightBB[2][64];
    

    // tables for bishops and rooks
    GMagic mBishopTbl[64];
    GMagic mRookTbl[64];

    // precomputed attack tables
    U64 bishopAttacks[2048];
    U64 rookAttacks[8196];

    void generateKnightAndKingMoves();
    void generateSlidingPieceMoves();


    void genereateBishopBlockerPermutations();
    void genereateRookBlockerPermutations();
    // void updateBishopAttackTable(int square);
    // void updateRookAttackTable(int square);

    void updateBitboard(U64 &bitboard, int row, int col, int move[2]);
    void updatePawnMoves(U64 &whitePawnMoves, U64 &blackPawnMoves, int row, int col);
    void updatePawnCaptures(U64 &whiteCaptures, U64 &blackCaptures, int row, int col);
    void updateRookMoves(U64 &rookMoves, int row, int col);
    void updateBishopMoves(U64 &bishopMoves, int row, int col);
    void generatePawnMoves();

    U64 getBishopAttacks(U64 occupancy, int square);
    U64 getRookAttacks(U64 occupancy, int square);
public:


    MoveGenerator();

    // U64 getPawnMoves(int square) const {};    
    // U64 getKnightMoves(int square) const { return knightMoves[square]; };  
    // U64 getBishopMoves(int square) const {};  
    // U64 getRookMoves(int square) const {};     
    // U64 getQueenMoves(int square) const {};    
    // U64 getKingMoves(int square) const {};  

    Move generateAllLegalMoves(const Board& board);    // generates an array of all legal moves
    void generateLegalMoves(const Board& board, int square, enumPiece pieceType);    // generates bitmap of all legal moves for that pieces

    U16 readMove(uint8_t from, uint8_t to, const Board& board);     // returns move int
    U16 getMove(int i) { return moveStruct.moveList[i];}

    enumPiece getPromotionPiece() const{ return nQueens;}    // just return queen for now

    void displayBitboard(U64 bitboard, int square, char symbol) const;
    
    void displayAllLegalMoves() const;
    void displayLegalMove(const U16& move) const;

};

#endif // MOVE_H
