#include "Board.h"
#include <sstream>

Board::Board() {
    grid.resize(8, std::vector<Piece>(8));
    reset();
}

const Piece& Board::getPiece(int row, int col) const {
    return grid[row][col];
}

void Board::setPiece(int row, int col, const Piece& piece) {
    grid[row][col] = piece;
}

void Board::reset() {
    // egyszerű kezdőállás: csak királyok
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            grid[r][c] = Piece();
        }
    }
    grid[0][4] = Piece(PieceType::King, Color::Black);
    grid[7][4] = Piece(PieceType::King, Color::White);
}

std::string Board::toString() const {
    std::ostringstream oss;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            const Piece& p = grid[r][c];
            char symbol = '.';
            if (p.type == PieceType::King) symbol = (p.color == Color::White) ? 'K' : 'k';
            oss << symbol << " ";
        }
        oss << "\n";
    }
    return oss.str();
}
