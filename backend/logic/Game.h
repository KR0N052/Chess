#pragma once
#include "Board.h"
#include "Piece.h"
#include "Move.h"
#define EMPTY 0

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
    bool wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const;
};
