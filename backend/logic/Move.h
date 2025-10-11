#pragma once
#include "pieces/Piece.h"

enum class Color { White, Black };
enum class PieceType { King, Queen, Bishop, Knight, Rook, Pawn, None };

struct Move {
    int fromRow, fromCol;
    int toRow, toCol;
    PieceType promotion = PieceType::None;
	bool isPromotion() const { return promotion != PieceType::None; }
    bool isCastle = false;
    bool isEnPassant = false;
    Move(int fr, int fc, int tr, int tc)
        : fromRow(fr), fromCol(fc), toRow(tr), toCol(tc) {}
};
