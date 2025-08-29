#pragma once
#include "Board.h"
#include "Move.h"

class MoveValidator {
public:
    static bool isValidMove(const Board& board, const Move& move, Color currentPlayer);
};
