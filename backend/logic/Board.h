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

    void clear() {
        for (auto& row : squares) {
            for (auto& cell : row) {
                cell = nullptr;
            }
        }
    }

    std::shared_ptr<Piece> getPiece(int row, int col) const {
        return squares[row][col];
    }

    void setPiece(int row, int col, std::shared_ptr<Piece> piece) {
        squares[row][col] = piece;
    }

    void movePiece(int fromRow, int fromCol, int toRow, int toCol) {
        squares[toRow][toCol] = squares[fromRow][fromCol];
        squares[fromRow][fromCol] = NULL;
    }

    int getSize() const {
        return 8;
    }
};
