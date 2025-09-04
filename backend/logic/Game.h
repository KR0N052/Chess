#pragma once
#include "Board.h"
#include "Piece.h"
#include "Move.h"

class Game {
private:
    Board board;
    Color currentTurn;

public:
    Game();
    void resetBoard();
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    Color getCurrentTurn() const;
    const Board& getBoard() const;
};
