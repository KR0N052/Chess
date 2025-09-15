// Rook.h
#pragma once
#include "Piece.h"
#include "Board.h"

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {
        ID = 'r';
    }

    std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const override {
        std::vector<Move> moves;
        int directions[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };

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
