#pragma once
#include "Board.h"
#include "MoveValidator.h"

class Game {
public:
    Game();
    bool makeMove(const Move& move);
    const Board& getBoard() const;
    Color getCurrentPlayer() const;

private:
    Board board;
    Color currentPlayer;
    void switchPlayer();
};
