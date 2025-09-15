#pragma once
#include "Piece.h"
#include "Board.h"

class King : public Piece {
public:
    King(Color c) : Piece(c) {
        ID = 'k';
    }

    std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const override {
        std::vector<Move> moves;
        int directions[8][2] = {
            {-1,-1}, {-1,0}, {-1,1},
            {0,-1},          {0,1},
            {1,-1},  {1,0},  {1,1}
        };

        for (auto& d : directions) {
            int newRow = row + d[0];
            int newCol = col + d[1];
            if (board.isInside(newRow, newCol)) {
                auto target = board.getPiece(newRow, newCol);
                if (!target || target->getColor() != color) {
                    moves.push_back({ row, col, newRow, newCol });
                }
            }
        }
        return moves;
    }
};
