#include "Board.h"

#include <sstream>
#include "Piece.h"
#include "pieces/Piece.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"

bool Board::isInside(int row, int col) const {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

void Board::clear() {
    for (auto& row : squares) {
        for (auto& cell : row) {
            cell = nullptr;
        }
    }
}

std::shared_ptr<Piece> Board::getPiece(int row, int col) const {
    return squares[row][col];
}

void Board::setPiece(int row, int col, std::shared_ptr<Piece> piece) {
    squares[row][col] = piece;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    squares[toRow][toCol] = squares[fromRow][fromCol];
    squares[fromRow][fromCol] = NULL;
}

void Board::reset() {
    clear();
    // --- Fekete bábuk felül ---
    setPiece(0, 4, std::make_shared<King>(Color::Black));

    setPiece(0, 3, std::make_shared<Queen>(Color::Black));

    setPiece(0, 0, std::make_shared<Rook>(Color::Black));
    setPiece(0, 7, std::make_shared<Rook>(Color::Black));

    setPiece(0, 1, std::make_shared<Knight>(Color::Black));
    setPiece(0, 6, std::make_shared<Knight>(Color::Black));

    setPiece(0, 2, std::make_shared<Bishop>(Color::Black));
    setPiece(0, 5, std::make_shared<Bishop>(Color::Black));

    for (int col = 0; col < 8; ++col) {
        setPiece(1, col, std::make_shared<Pawn>(Color::Black));
    }

    // --- Fehér bábuk alul ---
    setPiece(7, 4, std::make_shared<King>(Color::White));

    setPiece(7, 3, std::make_shared<Queen>(Color::White));

    setPiece(7, 0, std::make_shared<Rook>(Color::White));
    setPiece(7, 7, std::make_shared<Rook>(Color::White));

    setPiece(7, 1, std::make_shared<Knight>(Color::White));
    setPiece(7, 6, std::make_shared<Knight>(Color::White));

    setPiece(7, 2, std::make_shared<Bishop>(Color::White));
    setPiece(7, 5, std::make_shared<Bishop>(Color::White));

    for (int col = 0; col < 8; ++col) {
        setPiece(6, col, std::make_shared<Pawn>(Color::White));
    }
}

void Board::load(const std::string& str) {
    std::istringstream iss(str);
    std::string line;
    int row = 0;

    clear();

    while (std::getline(iss, line)) {
        if (line.empty()) continue;

        std::istringstream lineStream(line);
        std::string token;
        int col = 0;

        while (lineStream >> token) {
            char c = token[0];
            if (c == '.') {
                // üres mező
            }
            else {
                Color color = isupper(c) ? Color::White : Color::Black;
                char pieceChar = static_cast<char>(tolower(c));
                std::shared_ptr<Piece> piece;

                switch (pieceChar) {
                case 'k': piece = std::make_shared<King>(color); break;
                case 'q': piece = std::make_shared<Queen>(color); break;
                case 'r': piece = std::make_shared<Rook>(color); break;
                case 'n': piece = std::make_shared<Knight>(color); break;
                case 'b': piece = std::make_shared<Bishop>(color); break;
                case 'p': piece = std::make_shared<Pawn>(color); break;
                default: break;
                }

                if (piece) {
                    setPiece(row, col, piece);
                }
            }
            ++col;
        }
        ++row;
    }
}

std::string Board::debugString() const {
    std::ostringstream oss;
    oss << "Board state:\n";
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto piece = getPiece(r, c);
            if (!piece) {
                oss << ".";
            }
            else {
                char ch = '?';
                switch (piece->getType()) {
                case PieceType::King:   ch = 'K'; break;
                case PieceType::Queen:  ch = 'Q'; break;
                case PieceType::Rook:   ch = 'R'; break;
                case PieceType::Bishop: ch = 'B'; break;
                case PieceType::Knight: ch = 'N'; break;
                case PieceType::Pawn:   ch = 'P'; break;
                default: break;
                }
                if (piece->getColor() == Color::Black)
                    ch = std::tolower(ch);
                oss << ch;
            }
            oss << " ";
        }
        oss << "\n";
    }
    return oss.str();
}