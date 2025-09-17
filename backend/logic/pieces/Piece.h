#pragma once
#include <vector>
#include "Move.h"

enum class Color { White, Black };
enum class PieceType { King, Queen, Bishop, Knight, Rook, Pawn, None };


class Board; // előre deklarálás

class Piece {
protected:
    Color color;
public:
    Piece(Color c) : color(c) {}
    virtual ~Piece() = default;

    Color getColor() const { return color; }

	virtual PieceType getType() const = 0;

    // Minden bábu implementálja a saját lépéslogikáját
    virtual std::vector<Move> getPossibleMoves(const Board& board, int row, int col) const = 0;
};
