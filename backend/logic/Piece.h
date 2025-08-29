#pragma once

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn, None };
enum class Color { White, Black, None };

struct Piece {
    PieceType type;
    Color color;

    Piece(PieceType t = PieceType::None, Color c = Color::None)
        : type(t), color(c) {}
};
