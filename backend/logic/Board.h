// Board.h
#pragma once
#include <memory>
#include <string>

class Piece; // előre deklarálás

class Board {
private:
    std::shared_ptr<Piece> squares[8][8];

public:
    bool isInside(int row, int col) const;

    void clear();

    std::shared_ptr<Piece> getPiece(int row, int col) const;

    void setPiece(int row, int col, std::shared_ptr<Piece> piece);

    void movePiece(int fromRow, int fromCol, int toRow, int toCol);

    void applyMove(const struct Move& move);

    void reset();

    void load(const std::string& str);

    std::string debugString() const;
};
