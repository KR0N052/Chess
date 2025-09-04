#include "Game.h"
#include "King.h"
// Ha a többi bábu még nem lett átalakítva, akkor a régi PieceType/Color enumokat is be lehet húzni
#include <algorithm>
#include <stdexcept>

Game::Game() {
    resetBoard();
    currentTurn = Color::White;
}

void Game::resetBoard() {
    // Tábla ürítése
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            board.setPiece(r, c, nullptr);
        }
    }

    // Királyok elhelyezése az új logikával
    board.setPiece(0, 4, std::make_shared<King>(Color::White));
    board.setPiece(7, 4, std::make_shared<King>(Color::Black));

    // A többi bábu ideiglenesen maradhat a régi rendszerben
    // Például: gyalogok, bástyák, stb. — itt majd később cseréljük le őket
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) {
        return false; // nincs bábu ezen a mezőn
    }

    if (piece->getColor() != currentTurn) {
        return false; // nem a soron következő játékos bábuját próbálja mozgatni
    }

    // Új logika: a bábu maga adja vissza a lehetséges lépéseit
    auto possibleMoves = piece->getPossibleMoves(board, fromRow, fromCol);

    bool found = std::any_of(possibleMoves.begin(), possibleMoves.end(),
        [&](const Move& m) {
            return m.toRow == toRow && m.toCol == toCol;
        });

    if (!found) {
        return false; // érvénytelen lépés
    }

    // Lépés végrehajtása
    board.setPiece(toRow, toCol, piece);
    board.setPiece(fromRow, fromCol, nullptr);

    // Körváltás
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
    return true;
}

Color Game::getCurrentTurn() const {
    return currentTurn;
}

const Board& Game::getBoard() const {
    return board;
}
