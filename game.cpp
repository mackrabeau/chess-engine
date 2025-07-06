#include "game.h"
#include "movegenerator.h"

using namespace std;

// doMove: modifies the board in place
void Game::doMove(const U16& move) {
    // Save the current board state to history
    BoardState prev;
    memcpy(prev.pieceBB, board.pieceBB, 8 * sizeof(U64));
    prev.gameInfo = board.gameInfo;
    history.push_back(prev);

    Board::applyMove(move, board.pieceBB, board.gameInfo, board);
}

void Game::undoMove() {
    if (history.empty()) return; // Nothing to undo

    BoardState prev = history.back();
    history.pop_back();

    memcpy(board.pieceBB, prev.pieceBB, 8 * sizeof(U64));
    board.gameInfo = prev.gameInfo;
}

// makeMove: returns a new Board
Board Game::makeMove(const U16& move) {
    U64 newPieceBB[8];
    memcpy(newPieceBB, board.pieceBB, 8 * sizeof(U64));
    U16 newGameInfo = board.gameInfo;

    Board::applyMove(move, newPieceBB, newGameInfo, board);

    return Board(newPieceBB, newGameInfo);
}


void Game::updateGameState() {
    MoveGenerator moveGen(MoveTables::instance());
    Move legalMoves = moveGen.generateAllLegalMoves(board);

    if (legalMoves.count == 0) {
        if (board.isInCheck()) {
            state = CHECKMATE;
        } else {
            state = STALEMATE;
        }
    } else if (isThreefoldRepetition()) {
            state = DRAW_INSUFFICIENT_MATERIAL;
    } else if (isFiftyMoveRule()) {
            state = DRAW_50_MOVE;
    } else if (isInsufficientMaterial()) {
            state = DRAW_INSUFFICIENT_MATERIAL;
    } else {
        state = ONGOING;
    }
}

