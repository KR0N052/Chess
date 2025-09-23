#pragma once
#include "Board.h"
#include "Piece.h"
#include "Move.h"
#include <optional>

#define EMPTY 0



class Game {
public:
    Game();
    void resetBoard();
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);
    bool wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const;
    Color getCurrentTurn() const;
    const Board& getBoard() const;
    std::vector<Move> getLegalMoves(int fromRow, int fromCol) const;

private:
    Board board;
    Color currentTurn;
    std::pair<int, int> whiteKingPos;
    std::pair<int, int> blackKingPos;

    std::optional<std::pair<int, int>> enPassantTarget;

    void addCastlingMoves(int row, int col, std::vector<Move>& moves) const;
    void addEnPassantMoves(int row, int col, std::vector<Move>& moves) const;
    void addPromotionMoves(std::vector<Move>& moves) const;

    // aktív bábuk listája
    std::vector<std::shared_ptr<Piece>> whitePieces;
    std::vector<std::shared_ptr<Piece>> blackPieces;

};
