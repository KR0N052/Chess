#include "Game.h"
#include "pieces/Piece.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "Board.h"
#include <memory>
#include <vector>
#include <cmath>


// ---------------- Implementáció ----------------
Game::Game() {
    resetBoard();
}


void Game::resetBoard() {
    board.clear();

    // --- Fekete bábuk felül ---
    board.setPiece(0, 0, std::make_shared<Rook>(Color::Black));
    board.setPiece(0, 1, std::make_shared<Knight>(Color::Black));
    board.setPiece(0, 2, std::make_shared<Bishop>(Color::Black));
    board.setPiece(0, 3, std::make_shared<Queen>(Color::Black));
    board.setPiece(0, 4, std::make_shared<King>(Color::Black));
    board.setPiece(0, 5, std::make_shared<Bishop>(Color::Black));
    board.setPiece(0, 6, std::make_shared<Knight>(Color::Black));
    board.setPiece(0, 7, std::make_shared<Rook>(Color::Black));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(1, col, std::make_shared<Pawn>(Color::Black));
    }

    // --- Fehér bábuk alul ---
    board.setPiece(7, 0, std::make_shared<Rook>(Color::White));
    board.setPiece(7, 1, std::make_shared<Knight>(Color::White));
    board.setPiece(7, 2, std::make_shared<Bishop>(Color::White));
    board.setPiece(7, 3, std::make_shared<Queen>(Color::White));
    board.setPiece(7, 4, std::make_shared<King>(Color::White));
    board.setPiece(7, 5, std::make_shared<Bishop>(Color::White));
    board.setPiece(7, 6, std::make_shared<Knight>(Color::White));
    board.setPiece(7, 7, std::make_shared<Rook>(Color::White));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(6, col, std::make_shared<Pawn>(Color::White));
    }

    // Fehér kezd
    currentTurn = Color::White;
    lastMove = { -1, -1, -1, -1, nullptr };
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;

    if (piece->getColor() != currentTurn) return false;

    auto moves = piece->getPossibleMoves(board, fromRow, fromCol);

    std::vector<Move> legalMoves;
    for (const auto& move : moves) {
        if (!wouldBeInCheckAfterMove(fromRow, fromCol, move.toRow, move.toCol)) {
            legalMoves.push_back(move);
        }
    }

    bool validMove = false;
    for (const auto& move : legalMoves) {
        if (move.toRow == toRow && move.toCol == toCol) {
            validMove = true;
            break;
        }
    }

    // --- En passant ---
    if (piece->getType() == PieceType::Pawn) {
        if (toCol != fromCol && !board.getPiece(toRow, toCol)) {
            if (lastMove.piece && lastMove.piece->getType() == PieceType::Pawn) {
                if (lastMove.toRow == fromRow && lastMove.toCol == toCol &&
                    std::abs(lastMove.fromRow - lastMove.toRow) == 2) {
                    board.setPiece(lastMove.toRow, lastMove.toCol, nullptr);
                    validMove = true;
                }
            }
        }
    }

    // --- Castling ---
    if (piece->getType() == PieceType::King) {
        auto king = static_cast<King*>(piece.get());
        if (fromRow == toRow && std::abs(toCol - fromCol) == 2) {
            int dir = (toCol > fromCol) ? 1 : -1;
            int rookCol = (dir == 1) ? 7 : 0;
            int rookTargetCol = fromCol + dir;

            auto rook = board.getPiece(fromRow, rookCol);
            if (rook && rook->getType() == PieceType::Rook) {
                if (!king->hasMoved() && !static_cast<Rook*>(rook.get())->hasMoved()) {
                    bool pathClear = true;
                    for (int c = fromCol + dir; c != rookCol; c += dir) {
                        if (board.getPiece(fromRow, c)) { pathClear = false; break; }
                    }
                    bool safe = true;
                    for (int step = 0; step <= 2; step++) {
                        int checkCol = fromCol + step * dir;
                        if (wouldBeInCheckAfterMove(fromRow, fromCol, fromRow, checkCol)) {
                            safe = false; break;
                        }
                    }
                    if (pathClear && safe) {
                        board.movePiece(fromRow, fromCol, toRow, toCol);
                        board.movePiece(fromRow, rookCol, fromRow, rookTargetCol);
                        king->setMoved(true);
                        static_cast<Rook*>(rook.get())->setMoved(true);
                        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
                        return true;
                    }
                }
            }
        }
    }

    if (!validMove) return false;

    lastMove = { fromRow, fromCol, toRow, toCol, piece };
    board.movePiece(fromRow, fromCol, toRow, toCol);

    auto movedPiece = board.getPiece(toRow, toCol);
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        int promotionRow = (movedPiece->getColor() == Color::White) ? 0 : 7;
        if (toRow == promotionRow) {
            board.setPiece(toRow, toCol, std::make_shared<Queen>(movedPiece->getColor()));
        }
    }

    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
    return true;
}

bool Game::wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const {
    Board tempBoard = board;
    tempBoard.movePiece(fromRow, fromCol, toRow, toCol);

    Color movingColor = board.getPiece(fromRow, fromCol)->getColor();

    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() == movingColor && p->getType() == PieceType::King) {
                kingRow = r; kingCol = c; break;
            }
        }
    }

    // --- Pawn attacks ---
    int dir = (movingColor == Color::White) ? -1 : 1;
    for (int dc : {-1, 1}) {
        int r = kingRow + dir, c = kingCol + dc;
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() != movingColor && p->getType() == PieceType::Pawn) return true;
        }
    }

    // Knight attacks
    int knightOffsets[8][2] = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},
        {1,-2},{1,2},{2,-1},{2,1}
    };
    for (auto& off : knightOffsets) {
        int r = kingRow + off[0], c = kingCol + off[1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() != movingColor && p->getType() == PieceType::Knight) {
                return true;
            }
        }
    }

    // Bishop / Queen diagonals
    int diagDirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    for (auto& d : diagDirs) {
        int r = kingRow + d[0], c = kingCol + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p) {
                if (p->getColor() != movingColor &&
                    (p->getType() == PieceType::Bishop || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break; // blokkolva
            }
            r += d[0]; c += d[1];
        }
    }

    // Rook / Queen lines
    int lineDirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& d : lineDirs) {
        int r = kingRow + d[0], c = kingCol + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p) {
                if (p->getColor() != movingColor &&
                    (p->getType() == PieceType::Rook || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // King (szomszédos mezők)
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = kingRow + dr, c = kingCol + dc;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                auto p = tempBoard.getPiece(r, c);
                if (p && p->getColor() != movingColor && p->getType() == PieceType::King) {
                    return true;
                }
            }
        }
    }

    return false; // nincs sakk
}

Color Game::getCurrentTurn() const {
	return currentTurn;
}


const Board& Game::getBoard() const {
	return board;
}