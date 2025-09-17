// Queen.h
#pragma once
#include "Piece.h"
#include "Board.h"

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c) {}

    PieceType getType() const override {
        return PieceType::Queen;
    }

    std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const override {
        std::vector<Move> moves;
        int directions[8][2] = {
            {-1,0},{1,0},{0,-1},{0,1},
            {-1,-1},{-1,1},{1,-1},{1,1}
        };

        for (auto& d : directions) {
            int r = row + d[0];
            int c = col + d[1];
            while (board.isInside(r, c)) {
                auto target = board.getPiece(r, c);
                if (!target) {
                    moves.push_back({ row, col, r, c });
                }
                else {
                    if (target->getColor() != color) {
                        moves.push_back({ row, col, r, c });
                    }
                    break;
                }
                r += d[0];
                c += d[1];
            }
        }

        return moves;
    }
};
