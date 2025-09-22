#pragma once
#include "Board.h"
#include "Piece.h"
#include "Move.h"
#define EMPTY 0



class Game {
public:
    Game();
    void resetBoard();
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const;
    Color getCurrentTurn() const;
    const Board& getBoard() const;

private:
    Board board;
    Color currentTurn;

    struct LastMove {
        int fromRow, fromCol, toRow, toCol;
        std::shared_ptr<Piece> piece;
    };
    LastMove lastMove;
};
