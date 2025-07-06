#include "movegenerator.h"
#include "board.h"

using namespace std;

U64 MoveGenerator::getRookAttacks(U64 occupancy, int square) {
    U64 attacks = 0ULL;
    int r = square / 8, c = square % 8;

    // up
    for (int i = r + 1; i < 8; ++i) {
        int sq = i * 8 + c;
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // down
    for (int i = r - 1; i >= 0; --i) {
        int sq = i * 8 + c;
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // right
    for (int i = c + 1; i < 8; ++i) {
        int sq = r * 8 + i;
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // left
    for (int i = c - 1; i >= 0; --i) {
        int sq = r * 8 + i;
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    return attacks;
}

U64 MoveGenerator::getBishopAttacks(U64 occupancy, int square) {
    U64 attacks = 0ULL;
    int r = square / 8, c = square % 8;

    // up-right
    for (int i = 1; r + i < 8 && c + i < 8; ++i) {
        int sq = (r + i) * 8 + (c + i);
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // up-left
    for (int i = 1; r + i < 8 && c - i >= 0; ++i) {
        int sq = (r + i) * 8 + (c - i);
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // down-right
    for (int i = 1; r - i >= 0 && c + i < 8; ++i) {
        int sq = (r - i) * 8 + (c + i);
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    // down-left
    for (int i = 1; r - i >= 0 && c - i >= 0; ++i) {
        int sq = (r - i) * 8 + (c - i);
        attacks |= 1ULL << sq;
        if (occupancy & (1ULL << sq)) break;
    }

    return attacks;
}


Move MoveGenerator::generateAllLegalMoves(const Board& board){    

    Move pseudoMoves;
    Move legalMoves;

    U64 pieces = board.getFriendlyPieces(); // get all friendly pieces

    while (pieces) {
        int square = __builtin_ctzll(pieces);  // get the least significant set bit
        pieces &= pieces - 1;                  // remove the least significant set bit

        enumPiece pieceType = board.getPieceType(square);
        if (pieceType != nEmpty) {
            generateMoves(pieceType, board, square, pseudoMoves);
        }
    }

    // Filter out moves that leave the king in check

    for (short i = 0; i < pseudoMoves.count; i++) {
        U16 move = pseudoMoves.moveList[i];

        Board tempBoard = board; // Create a temporary board to test moves
        Board::applyMove(move, tempBoard.pieceBB, tempBoard.gameInfo, tempBoard);
        if (!tempBoard.isInCheck(tempBoard.friendlyColour())) {
            legalMoves.addMove(move);
        }
    }

    return legalMoves;
};

void MoveGenerator::generateMoves(enumPiece pieceType, const Board& board, int square, Move& pseudoMoves){   
    switch (pieceType) {
        case nKings: generateKingMovesForSquare(board, square, pseudoMoves); break;
        case nKnights: generateKnightMovesForSquare(board, square, pseudoMoves); break;
        case nBishops: generateBishopMovesForSquare(board, square, pseudoMoves); break;
        case nRooks: generateRookMovesForSquare(board, square, pseudoMoves); break;
        case nQueens: generateQueenMovesForSquare(board, square, pseudoMoves); break;
        case nPawns: generatePawnMovesForSquare(board, square, pseudoMoves); break;
        default: break;
    }
};

void MoveGenerator::generateKingMovesForSquare(const Board& board, int square, Move& pseudoMoves) {

    U64 movesBB = tables.kingBB[square];
    U64 attacked = attackedBB(board, board.enemyColour()); // get attacked squares by enemy pieces
    U64 empty = ~(board.pieceBB[nWhite] | board.pieceBB[nBlack]); // all empty squares

    // White king on e1 (square 4)
    if (board.friendlyColour() == nWhite && square == 4) {
        if ((board.gameInfo & WK_CASTLE) && !(attacked & WK_CASTLE_MASK)) {
            // Check squares f1 (5) and g1 (6) are empty and not attacked, etc.
            movesBB |= (1ULL << 6); // g1
        }
        if ((board.gameInfo & WQ_CASTLE) && !(attacked & WQ_CASTLE_MASK)) {
            // Check squares d1 (3), c1 (2), and b1 (1) are empty and not attacked, etc.
            movesBB |= (1ULL << 2); // c1
        }
    }

    // Black king on e8 (square 60)
    else if (board.friendlyColour() == nBlack && square == 60) {
        if ((board.gameInfo & BK_CASTLE) && !(attacked & BK_CASTLE_MASK)) {
            movesBB |= (1ULL << 62); // g8
        }
        if ((board.gameInfo & BQ_CASTLE) && !(attacked & BQ_CASTLE_MASK)) {
            movesBB |= (1ULL << 58); // c8
        }
    }

    addMovesToStruct(pseudoMoves, board, square, movesBB);
}

void MoveGenerator::generateQueenMovesForSquare(const Board& board, int square, Move& pseudoMoves) {
    U64 movesBB = getBishopAttacks(board.getAllPieces(), square) | getRookAttacks(board.getAllPieces(), square);
    addMovesToStruct(pseudoMoves, board, square, movesBB);
}

void MoveGenerator::generateRookMovesForSquare(const Board& board, int square, Move& pseudoMoves) {
    U64 movesBB = getRookAttacks(board.getAllPieces(), square);
    addMovesToStruct(pseudoMoves, board, square, movesBB);
}

void MoveGenerator::generateBishopMovesForSquare(const Board& board, int square, Move& pseudoMoves) {
    U64 movesBB = getBishopAttacks(board.getAllPieces(), square);
    addMovesToStruct(pseudoMoves, board, square, movesBB);
}

void MoveGenerator::generateKnightMovesForSquare(const Board& board, int square, Move& pseudoMoves) {
    U64 movesBB = tables.knightBB[square];
    addMovesToStruct(pseudoMoves, board, square, movesBB);
}

void MoveGenerator::generatePawnMovesForSquare(const Board& board, int square, Move& pseudoMoves) {
    U64 empty = ~(board.pieceBB[nWhite] | board.pieceBB[nBlack]); // all empty squares
    U64 movesBB = tables.pawnMovesBB[board.friendlyColour()][square] & empty;

    int row = square / 8;
    int col = square % 8;

    // Double pawn push
    if (board.friendlyColour() == nWhite && row == 1 && !(movesBB & (1ULL << (square + 8)))) {
        movesBB &= ~(1ULL << (square + 16)); 
    } else if (board.friendlyColour() == nBlack && row == 6 && !(movesBB & (1ULL << (square - 8)))) {
        movesBB &= ~(1ULL << (square - 16)); // remove double pawn push if square is not empty
    }

    // Captures
    movesBB |= tables.pawnMovesCapturesBB[board.friendlyColour()][square] & (board.pieceBB[board.enemyColour()]);

    int epSquare = board.getEnPassantSquare(); // get en passant square if available
    if (epSquare != -1) {
        int epRow = epSquare / 8;
        int epCol = epSquare % 8;
        if (board.friendlyColour() == nWhite && row == 4 && abs(col - epCol) == 1 && epRow == 5)
            movesBB |= (1ULL << epSquare);
        if (board.friendlyColour() == nBlack && row == 3 && abs(col - epCol) == 1 && epRow == 2)
            movesBB |= (1ULL << epSquare);
    }

    addMovesToStruct(pseudoMoves, board, square, movesBB);
}



U64 MoveGenerator::attackedBB(const Board& board, U8 enemyColour) {
    // Check if the square is attacked by any enemy piece
    U64 pieces = board.pieceBB[enemyColour];
    U64 attacked = 0ULL;

    while (pieces) {
        int square = __builtin_ctzll(pieces);
        pieces &= pieces - 1;
        enumPiece type = board.getPieceType(square);

        switch (type) {
            case nPawns:
                attacked |= tables.pawnMovesCapturesBB[enemyColour][square];
                break;
            case nKnights:
                attacked |= tables.knightBB[square];
                break;
            case nBishops:
                attacked |= getBishopAttacks(board.getAllPieces(), square);
                break;
            case nRooks:
                attacked |= getRookAttacks(board.getAllPieces(), square);
                break;
            case nQueens:
                attacked |= getBishopAttacks(board.getAllPieces(), square);
                attacked |= getRookAttacks(board.getAllPieces(), square);
                break;
            case nKings:
                attacked |= tables.kingBB[square];
                break;
            default:
                break;
        }
    }
    return attacked;
}


void MoveGenerator::addMovesToStruct(Move& moves, const Board& board, int square, U64 movesBB) {
    // remove moves that land on friendly pieces
    movesBB ^= movesBB & board.pieceBB[board.friendlyColour()];
    
    while(movesBB) {
        int to = __builtin_ctzll(movesBB);
        movesBB &= movesBB - 1;
        U16 move = readMove(square, to, board);
        // std::cout << "addMove: from " << square << " to " << to << " flags: " << ((move >> 12) & 0xF) << std::endl;
        moves.addMove(move);
    }
}

U16 MoveGenerator::readMove(U8 from, U8 to, const Board& board){
    U16 move = (from << 6) | to;
    enumPiece piece = board.getPieceType(from);
    enumPiece target = board.getPieceType(to);

    // special moves 
    bool isPromotion = (piece == nPawns) && (to >= 56 || to <= 7);
    bool isCapture = (target != nEmpty);

    bool isEnPassant = false;
    if (piece == nPawns) {
        int epSquare = board.getEnPassantSquare();
        if (epSquare != -1 && to == epSquare && target == nEmpty) {
            isCapture = true; // en passant capture
            isEnPassant = true;
        }
    } 
    bool isDoublePawnPush = (piece == nPawns) && (abs((int)from - (int)to) == 16);
    bool isCastle = (piece == nKings) && (abs((int)from - (int)to) == 2);
    bool isQueenCastle = isCastle && (to == 2 || to == 58);

    // promotion piece selection (for now, always queen)
    int promotionType = 0; // 0=knight, 1=bishop, 2=rook, 3=queen
    if (isPromotion) {
        promotionType = 3; // assume user chose queen
    }
    
    if (isPromotion) {
        switch (promotionType) {
            case 0: move |= FLAG_PROMO_KNIGHT; break;
            case 1: move |= FLAG_PROMO_BISHOP; break;
            case 2: move |= FLAG_PROMO_ROOK;   break;
            case 3: move |= FLAG_PROMO_QUEEN;  break;
        }
        if (isCapture) move |= FLAG_CAPTURE;
    } else if (isCapture) {
        if (isEnPassant)
            move |= FLAG_EP_CAPTURE;
        else
            move |= FLAG_CAPTURE_MOVE;
    } else if (isDoublePawnPush) {
        move |= FLAG_DOUBLE_PAWN_PUSH;
    } else if (isCastle) {
        if (isQueenCastle)
            move |= FLAG_QUEEN_CASTLE;
        else
            move |= FLAG_KING_CASTLE;
    } 

    // otherwise, quiet move (no extra flags)

    return move;
}


void MoveGenerator::displayBitboard(U64 bitboard, int square, char symbol) const {   
    std::cout << "\n";    
    for (int r = 7; r >= 0; r --){
        for (int c = 0; c < 8; c ++){
            int square_i = r * 8 + c;
            char piece = '.';

            if (bitboard >> square_i & 1){
                piece = 'x';
            } 
            if (square_i == square){
                piece = symbol;
            }
            std::cout<< piece << " ";
        }
        std::cout<< "\n";
    }
}




