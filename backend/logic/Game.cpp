#include "Game.h"
#include "pieces/Piece.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include <memory> // std::shared_ptr, std::make_shared

Game::Game() {
    resetBoard();
}

void Game::resetBoard() {
    board.clear();

    // --- Fehér bábuk ---
    board.setPiece(0, 0, std::make_shared<Rook>(Color::White));
    board.setPiece(0, 1, std::make_shared<Knight>(Color::White));
    board.setPiece(0, 2, std::make_shared<Bishop>(Color::White));
    board.setPiece(0, 3, std::make_shared<Queen>(Color::White));
    board.setPiece(0, 4, std::make_shared<King>(Color::White));
    board.setPiece(0, 5, std::make_shared<Bishop>(Color::White));
    board.setPiece(0, 6, std::make_shared<Knight>(Color::White));
    board.setPiece(0, 7, std::make_shared<Rook>(Color::White));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(1, col, std::make_shared<Pawn>(Color::White));
    }

    // --- Fekete bábuk ---
    board.setPiece(7, 0, std::make_shared<Rook>(Color::Black));
    board.setPiece(7, 1, std::make_shared<Knight>(Color::Black));
    board.setPiece(7, 2, std::make_shared<Bishop>(Color::Black));
    board.setPiece(7, 3, std::make_shared<Queen>(Color::Black));
    board.setPiece(7, 4, std::make_shared<King>(Color::Black));
    board.setPiece(7, 5, std::make_shared<Bishop>(Color::Black));
    board.setPiece(7, 6, std::make_shared<Knight>(Color::Black));
    board.setPiece(7, 7, std::make_shared<Rook>(Color::Black));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(6, col, std::make_shared<Pawn>(Color::Black));
    }

    // Fehér kezd
    currentTurn = Color::White;
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false; // nincs bábu ezen a mezőn

    // 1. Ellenőrzés: csak a soron következő játékos léphet
    if (piece->getColor() != currentTurn) {
        return false;
    }

    // 2. Megnézzük, hogy a lépés benne van-e a bábu lehetséges lépéseiben
    auto moves = piece->getPossibleMoves(board, fromRow, fromCol);
    bool validMove = false;
    for (const auto& move : moves) {
        if (move.toRow == toRow && move.toCol == toCol) {
            validMove = true;
            break;
        }
    }
    if (!validMove) return false;

    // 3. Lépés végrehajtása
    board.movePiece(fromRow, fromCol, toRow, toCol);
    
    // 4. Játékosváltás
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;

    return true;
}

Color Game::getCurrentTurn() const {
    return currentTurn;
}

const Board& Game::getBoard() const {
    return board;
}
