#include "Game.h"

Game::Game() : currentPlayer(Color::White) {
    board.reset();
}

bool Game::makeMove(const Move& move) {
    if (MoveValidator::isValidMove(board, move, currentPlayer)) {
        Piece movingPiece = board.getPiece(move.fromRow, move.fromCol);
        board.setPiece(move.toRow, move.toCol, movingPiece);
        board.setPiece(move.fromRow, move.fromCol, Piece());
        switchPlayer();
        return true;
    }
    return false;
}

const Board& Game::getBoard() const {
    return board;
}

Color Game::getCurrentPlayer() const {
    return currentPlayer;
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == Color::White) ? Color::Black : Color::White;
}
