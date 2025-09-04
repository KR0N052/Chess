// Board.h
#pragma once
#include <memory>
#include "Piece.h"

class Board {
private:
    std::shared_ptr<Piece> squares[8][8];

public:
    bool isInside(int row, int col) const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    std::shared_ptr<Piece> getPiece(int row, int col) const {
        return squares[row][col];
    }

    void setPiece(int row, int col, std::shared_ptr<Piece> piece) {
        squares[row][col] = piece;
    }
};
