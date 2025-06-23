#include "moves.h"
#include "board.h"

using namespace std;

#include <bitset>

MoveGenerator::MoveGenerator() {
    generateKnightAndKingMoves();
    generatePawnMoves();
    generateSlidingPieceMoves();
}

void MoveGenerator::generateKnightAndKingMoves() {
    int setKnightMoves[8][2] = { {2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2} };
    int setKingMoves[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1},  {-1, -1}, {-1, 1} };
    
    for (int square = 0; square < 64; square++) {
        U64 knightMoves_i = 0ULL;
        U64 kingMoves_i = 0ULL;
        int col = square % 8, row = square / 8;
        
        for (int i = 0; i < 8; i++) {
            updateBitboard(knightMoves_i, row, col, setKnightMoves[i]);
            updateBitboard(kingMoves_i, row, col, setKingMoves[i]);
        }
        
        kingKnightBB[0][square] = knightMoves_i;
        kingKnightBB[1][square] = kingMoves_i;
    }
}

void MoveGenerator::generatePawnMoves() {
    for (int square = 0; square < 64; square++) {
        int col = square % 8, row = square / 8;
        U64 whitePawnMoves_i = 0ULL, blackPawnMoves_i = 0ULL;
        U64 whitePawnMovesCaptures_i = 0ULL, blackPawnMovesCaptures_i = 0ULL;
        
        updatePawnMoves(whitePawnMoves_i, blackPawnMoves_i, row, col);
        updatePawnCaptures(whitePawnMovesCaptures_i, blackPawnMovesCaptures_i, row, col);
        
        pawnMovesBB[nWhite][square] = whitePawnMoves_i;
        pawnMovesBB[nBlack][square] = blackPawnMoves_i;
        pawnMovesCapturesBB[nWhite][square] = whitePawnMovesCaptures_i;
        pawnMovesCapturesBB[nBlack][square] = blackPawnMovesCaptures_i;
    }
}

void MoveGenerator::generateSlidingPieceMoves() {
    for (int square = 0; square < 64; square++) {
        int col = square % 8, row = square / 8;
        
        U64 rookMoves_i = 0ULL, bishopMoves_i = 0ULL;
        
        for (int c = 0; c < 8; c++) rookMoves_i |= 1ULL << (row * 8 + c);
        for (int r = 0; r < 8; r++) rookMoves_i ^= 1ULL << (r * 8 + col);

        for (int i = 1; i < 8; i++) {
            if (row - i >= 0 && col - i >= 0) bishopMoves_i |= 1ULL << ((row - i) * 8 + col - i);
            if (row + i < 8 && col - i >= 0) bishopMoves_i |= 1ULL << ((row + i) * 8 + col - i);
            if (row - i >= 0 && col + i < 8) bishopMoves_i |= 1ULL << ((row - i) * 8 + col + i);
            if (row + i < 8 && col + i < 8) bishopMoves_i |= 1ULL << ((row + i) * 8 + col + i);
        }
    
        mBishopTbl[square].mask = bishopMoves_i & ~EDGE_MASK;
        mRookTbl[square].mask = rookMoves_i & ~EDGE_MASK;

        mBishopTbl[square].magic = BMagic[square];
        mRookTbl[square].magic = RMagic[square];

        mBishopTbl[square].bits = BBits[square];
        mRookTbl[square].bits = RBits[square];
    }
}

void MoveGenerator::genereateBishopBlockerPermutations() {}
void MoveGenerator::genereateRookBlockerPermutations() {}



void MoveGenerator::updateRookMoves(U64 &rookMoves, int row, int col) {

}

void MoveGenerator::updateBishopMoves(U64 &bishopMoves, int row, int col) {

}

void MoveGenerator::updateBitboard(U64 &bitboard, int row, int col, int move[2]) {
    int newRow = row + move[0], newCol = col + move[1];
    if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
        bitboard |= 1ULL << (newRow * 8 + newCol);
    }
}

void MoveGenerator::updatePawnMoves(U64 &whitePawnMoves, U64 &blackPawnMoves, int row, int col) {
    if (row - 1 >= 0) whitePawnMoves |= 1ULL << ((row - 1) * 8 + col);
    if (row == 6) whitePawnMoves |= 1ULL << ((row - 2) * 8 + col);
    if (row + 1 < 8) blackPawnMoves |= 1ULL << ((row + 1) * 8 + col);
    if (row == 1) blackPawnMoves |= 1ULL << ((row + 2) * 8 + col);
}

void MoveGenerator::updatePawnCaptures(U64 &whiteCaptures, U64 &blackCaptures, int row, int col) {
    if (col + 1 < 8 && row - 1 >= 0) whiteCaptures |= 1ULL << ((row - 1) * 8 + col + 1);
    if (col - 1 >= 0 && row - 1 >= 0) whiteCaptures |= 1ULL << ((row - 1) * 8 + col - 1);
    if (col - 1 >= 0 && row + 1 < 8) blackCaptures |= 1ULL << ((row + 1) * 8 + col - 1);
    if (col + 1 < 8 && row + 1 < 8) blackCaptures |= 1ULL << ((row + 1) * 8 + col + 1);
}


U64 MoveGenerator::getBishopAttacks(U64 occupancy, int square){
    occupancy = occupancy & mBishopTbl[square].mask;
    occupancy *= mBishopTbl[square].magic;
    occupancy >>= (64 - mBishopTbl[square].bits);
    return bishopAttacks[occupancy];
}

U64 MoveGenerator::getRookAttacks(U64 occupancy, int square){
    occupancy = occupancy & mRookTbl[square].mask;
    occupancy *= mRookTbl[square].magic;
    occupancy >>= (64 - mRookTbl[square].bits);
    return rookAttacks[occupancy];
}


Move MoveGenerator::generateAllLegalMoves(const Board& board){    
    
    enumPiece coloursTurn = board.coloursTurn();

    U64 pieces;
    coloursTurn == nWhite ? pieces = board.pieceBB[nWhite] : pieces = board.pieceBB[nBlack];

    while (pieces) {
        int square = __builtin_ctzll(pieces);  // get the least significant set bit

        pieces &= pieces - 1;                  // remove the least significant set bit

        enumPiece pieceType = board.getPieceType(square);
        if (pieceType != nEmpty){
            generateLegalMoves(board, square, pieceType);
        }
    }
    return moveStruct;
};

void MoveGenerator::generateLegalMoves(const Board& board, int square, enumPiece pieceType){    
    U64 moves;

    // gets all possible (maybe not legal) moves --> ignores pins and when in check
    if (pieceType == nPawns){
        moves = pawnMovesBB[board.friendlyColour()][square] ^ (pawnMovesBB[board.friendlyColour()][square] & board.pieceBB[board.friendlyColour()]);
        moves |= pawnMovesCapturesBB[board.friendlyColour()][square] & board.pieceBB[board.enemyColour()]; 

        // deal with enpassant HERE
    } else if (pieceType == nKings) {
        moves = kingKnightBB[0][square];
    } else if (pieceType == nKnights) {
        moves = kingKnightBB[1][square];
    } else if (pieceType == nBishops){
        moves = getBishopAttacks(board.getAllPieces(), square);
    } else if (pieceType == nRooks){
        moves = getRookAttacks(board.getAllPieces(), square);
    } else if (pieceType == nQueens){
        moves = getBishopAttacks(board.getAllPieces(), square) | getRookAttacks(board.getAllPieces(), square);
    }


    // removes all moves on the same square as piece of the same colour
    // coloursTurn == White ? moves ^= moves & board.pieceBB[nWhite] : moves ^= moves & board.pieceBB[nBlack];
    moves ^= moves & board.pieceBB[board.friendlyColour()];

    
    // if (pieceType == Pawn){
    // displayBitboard(moves, square, board.pieceToChar(square));
    // }

    while(moves) {
        int to = __builtin_ctzll(moves);
        moves &= moves - 1;

        moveStruct.addMove(readMove(square, to, board));
    }
};

// USE MASKS instead of magic numbers
    // constexpr U16 CAPTURE_FLAG = (1 << 15);
    // constexpr U16 PROMOTION_FLAG = (1 << 16);
    // constexpr U16 DOUBLE_PAWN_PUSH_FLAG = (1 << 13);
    // constexpr U16 CASTLE_FLAG = (1 << 14);
    // constexpr U16 QUEEN_SIDE_CASTLE_FLAG = (1 << 13);
U16 MoveGenerator::readMove(uint8_t from, uint8_t to, const Board& board){
    // first 12 bit are two and from squares
    U16 move = (from << 6) | to;
    enumPiece piece = board.getPieceType(from);
    enumPiece squareMoved = board.getPieceType(to);


    // set capture flag
    if (squareMoved != nEmpty){ 
        move |= (1 << 15); 
    } 
    
    if (piece == nPawns){
        if (to >= 56 || to <= 7){
            // set promotion flag
            move |= (1 << 16);      

            enumPiece promotedPiece = getPromotionPiece();
            move |= (promotedPiece - nKnights) << 13;
        
        } else if ( abs(from - to) == 16 ){
            // double pawn push
            move |= (1 << 13);
        } 
        // FIX THIS, FIGURE OUT WAY TO DETECT ENPASSANT BETTER
        // else if ( squareMoved == EnPassant ){
        //     move |= (1 << 13);
        // }

    } else if (piece == nKings){
        if ( abs(from - to) == 2 ){
            // castle
            move |= (1 << 14); 
            if (to == 58 || to == 2){ // queen side
                move |= (1 << 13); 
            }
        }
    }

    return move;
}



void MoveGenerator::displayBitboard(U64 bitboard, int square, char symbol) const {   

    std::cout << "\n";
    
    for (int r = 0; r < 8; r ++){
        for (int c = 0; c < 8; c ++){

            int square_i = r * 8 + c;

            char piece = '.';

            if (bitboard >> square_i & 1){
                piece = 'x';
            } 

            if (square_i == square){
                piece = symbol;
                // piece = 'X';
            }

            std::cout<< piece << " ";

        }
        std::cout<< "\n";
    }
}

void MoveGenerator::displayAllLegalMoves() const {   

    int i = 0;
    while (moveStruct.moveList[i]){
        displayLegalMove(moveStruct.moveList[i]);

        i++;
    }
}

void MoveGenerator::displayLegalMove(const U16& move) const {   

    int flags = move >> 12;
    int from = move >> 6 & 0x3f;
    int to = move & 0x3f;
    
    std::cout << "flags: " << flags << ", from : " << from << ", to : " << to << "\n";

     
}

