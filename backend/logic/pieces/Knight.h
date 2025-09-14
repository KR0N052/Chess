// Knight.h
#pragma once
#include "Piece.h"
#include "Board.h"

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c) {}

    std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const override {
        std::vector<Move> moves;
        int offsets[8][2] = {
            {-2,-1},{-2,1},{-1,-2},{-1,2},
            {1,-2},{1,2},{2,-1},{2,1}
        };

        for (auto& o : offsets) {
            int r = row + o[0];
            int c = col + o[1];
            if (board.isInside(r, c)) {
                auto target = board.getPiece(r, c);
                if (!target || target->getColor() != color) {
                    moves.push_back({ row, col, r, c });
                }
            }
        }
        return moves;
    }
};
