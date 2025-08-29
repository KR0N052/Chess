#pragma once
#include "Piece.h"
#include <vector>
#include <string>

class Board {
public:
    Board();
    const Piece& getPiece(int row, int col) const;
    void setPiece(int row, int col, const Piece& piece);
    void reset();
    std::string toString() const; // debug célra

private:
    std::vector<std::vector<Piece>> grid;
};
