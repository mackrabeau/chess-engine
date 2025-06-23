#include "board.h"
#include "moves.h" 

int main() {

    Board board("r2qkb1r/pppn1ppp/2Q2n2/3pp1B1/2PP4/5N2/PP2PPPP/RN2KB1R w KQkq - 1 7");

    // Board board;
    // moveGen.displayAllLegalMoves();

    // initial board
    board.displayBoard();

    // get move
    MoveGenerator moveGen;
    moveGen.generateAllLegalMoves(board);
    moveGen.displayLegalMove(moveGen.getMove(2));

    // make first move
    board = board.makeMove(moveGen.getMove(2));
    board.displayBoard();

    // get second move
    MoveGenerator moveGen2;
    moveGen2.generateAllLegalMoves(board);
    moveGen2.displayLegalMove(moveGen2.getMove(1));

    // make second move
    board = board.makeMove(moveGen2.getMove(1));
    board.displayBoard();
        
    std::cout << "\n";

    return 0;
}
