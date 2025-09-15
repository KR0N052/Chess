// Pawn.h
#pragma once
#include "Piece.h"
#include "Board.h"

class Pawn : public Piece {
private:
    bool moved = false;
public:
    Pawn(Color c) : Piece(c) {
        ID = 'p';
    }


    std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const override {
        std::vector<Move> moves;
        int dir = (color == Color::White) ? 1 : -1; // fehér felfelé, fekete lefelé

        // Egy mező előre
        int newRow = row + dir;
        if (board.isInside(newRow, col) && !board.getPiece(newRow, col)) {
            moves.push_back({ row, col, newRow, col });

            // Két mező előre az első lépésnél
            if ((color == Color::White && row == 1) || (color == Color::Black && row == 6)) {
                int twoRow = row + 2 * dir;
                if (!board.getPiece(twoRow, col)) {
                    moves.push_back({ row, col, twoRow, col });
                }
            }
        }

        // Ütés átlósan
        for (int dc : {-1, 1}) {
            int newCol = col + dc;
            if (board.isInside(newRow, newCol)) {
                auto target = board.getPiece(newRow, newCol);
                if (target && target->getColor() != color) {
                    moves.push_back({row, col, newRow, newCol });
                }
            }
        }

        return moves;
    }
};
