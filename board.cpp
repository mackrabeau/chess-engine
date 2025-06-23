
#include "board.h"

using namespace std;

Board::Board(const std::string& fen){

    clearBoard();

    short state = 0;
    int square = 0;

    for(int i = 0; i < fen.length(); i++){
        if (state == 0){
            if (fen[i] == '/') continue;

            if (isdigit(fen[i])) {
                square += fen[i] - '0';
            } else if (fen[i] == ' '){
                state++;
            } else {
                loadPiece(fen[i], square);
                square++;
            }
            if (square == 64){
                state++;
            }
        } else if (state == 1){
            if (fen[i] == ' ') continue;
            
            gameInfo = fen[i] == 'w';
            
            state++;
            // add state to account for move counter and castling rights
        } else if (state == 2){
        }

    }
};

Board::Board(const Board& other){
    for (int i = 0; i < 8; i++){
        pieceBB[i] = other.pieceBB[i];
    }
    gameInfo = other.gameInfo;
}

Board::Board(U64 otherPieceBB[8], const U16& otherGameInfo){
    memcpy(pieceBB, otherPieceBB, 8 * sizeof(U64));
    gameInfo = otherGameInfo;
}

// assume move is legal
Board Board::makeMove(const U16& move){

    U64 newPieceBB[8];
    memcpy(newPieceBB, pieceBB, 8 * sizeof(U64));

    U16 newGameInfo;

    U8 from = getFrom(move);
    U8 to = getTo(move);
    U8 flags = getFlags(move);
    
    enumPiece pieceType = getPieceType(from);
    enumPiece colourType = getColourType(from);

    newPieceBB[pieceType] &= ~(1ULL << from);  // clear "from" square
    newPieceBB[pieceType] |= (1ULL << to);     // set "to" square

    newPieceBB[colourType] &= ~(1ULL << from);
    newPieceBB[colourType] |= (1ULL << to);

    newGameInfo = gameInfo ^ (1ULL << 0);  // change colours

    // // castling rights logic
    // if (pieceType == nKings){
    //     // set both king side and queen side castling to zero, 
    //     // shift and extra two bits depending on colour
    //     newGameInfo = gameInfo & ~( 0b11 << (1 + 2 * (gameInfo & 1ULL)));

    // }  else if (from == 56){          // top left square
    //     newGameInfo &= ~(1ULL <<1);   // queen side black

    // }  else if (from == 64){          // top right square
    //     newGameInfo &= ~(1ULL <<2);   // king side black

    // }  else if (from == 7){            // bottom right square
    //     newGameInfo &= ~(1ULL <<3);    // queen side white

    // }  else if (from == 0){            // bottom left square
    //     newGameInfo &= ~(1ULL <<4);    // king side white
    // }

    // // move counter logic
    // if (pieceType != nPawns){ // must check if in check
    //     newGameInfo = gameInfo + (1ULL << 5);   // increment move counter
    // } else {
    //     newGameInfo = gameInfo & 0x1F;          // reset move counter
    // }

    Board board = Board(newPieceBB, newGameInfo);

    return board;
}

void Board::clearBoard(){
    for (int i = 0; i < 8; i++) {
        pieceBB[i] = 0;  // clear all the bitboards
    }
}

void Board::displayBoard() const {
    for (int r = 0; r < 8; r ++){
        for (int c = 0; c < 8; c ++){

            int square = r * 8 + c;
            std::cout<< pieceToChar(square) << " ";
        }
        std::cout<< "\n";
    }
    coloursTurnToString();
}


enumPiece Board::coloursTurn() const{
    return (gameInfo & 1ULL) ? nBlack : nWhite;
} 

void Board::coloursTurnToString() const{
    string colour;
    (gameInfo & 1ULL) ? colour = "Black" : colour = "White" ;
    std::cout << colour << " to move!" << "\n\n";
}

void Board::loadPiece(char piece, int square) {
    
    if (piece == toupper(piece)) {
        pieceBB[nWhite] = pieceBB[nWhite] | (1ULL << square);
    } else {
        pieceBB[nBlack] = pieceBB[nBlack] | (1ULL << square);
    }

    piece = toupper(piece);
    
    if (piece == 'P') {pieceBB[nPawns] = pieceBB[nPawns] | (1ULL << square); return;}
    if (piece == 'N') {pieceBB[nKnights] = pieceBB[nKnights] | (1ULL << square); return;}
    if (piece == 'B') {pieceBB[nBishops] = pieceBB[nBishops] | (1ULL << square); return;}
    if (piece == 'R') {pieceBB[nRooks] = pieceBB[nRooks] | (1ULL << square); return;}
    if (piece == 'Q') {pieceBB[nQueens] = pieceBB[nQueens] | (1ULL << square); return;}
    if (piece == 'K') {pieceBB[nKings] = pieceBB[nKings] | (1ULL << square); return;}
}

enumPiece Board::getPieceType(int square) const {
    if ((getWhitePawns() | getBlackPawns()) >> square & 1){return nPawns;}
    if ((getWhiteBishops() | getBlackBishops())>> square & 1){return nBishops;}
    if ((getWhiteKnights() | getBlackKnights()) >> square & 1){return nKnights;}
    if ((getWhiteRooks() | getBlackRooks()) >> square & 1){return nRooks;}
    if ((getWhiteQueens() | getBlackQueens()) >> square & 1){return nQueens;}
    if ((getWhiteKing() | getBlackKing()) >> square & 1){return nKings;}
    return nEmpty;
}

enumPiece Board::getColourType(int square) const {
    if (pieceBB[nWhite] >> square & 1){return nWhite;}
    if (pieceBB[nBlack] >> square & 1){return nBlack;}

    throw std::invalid_argument( "received empty square");
}

char Board::pieceToChar(int square) const {
    if (getWhitePawns() >> square & 1){return 'P';}
    if (getBlackPawns() >> square & 1){return 'p';}

    if (getWhiteBishops() >> square & 1){return 'B';}
    if (getBlackBishops() >> square & 1){return 'b';}

    if (getWhiteKnights() >> square & 1){return 'N';}
    if (getBlackKnights() >> square & 1){return 'n';}

    if (getWhiteRooks() >> square & 1){return 'R';}
    if (getBlackRooks() >> square & 1){return 'r';}

    if (getWhiteQueens() >> square & 1){return 'Q';}
    if (getBlackQueens() >> square & 1){return 'q';}

    if (getWhiteKing() >> square & 1){return 'K';}
    if (getBlackKing() >> square & 1){return 'k';}
    return '.';
}

int Board::colourCode(enumPiece ct) const {
    return static_cast<int>(ct);
}

int Board::pieceCode(enumPiece pt) const {
    return static_cast<int>(pt);
}




