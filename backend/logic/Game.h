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


    Color getCurrentTurn() const;
    const Board& getBoard() const;
    std::vector<Move> getLegalMoves(int fromRow, int fromCol) const;

	bool isCheckmate() const;
	bool isStalemate() const;

private:
    bool fastWouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool safeWouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const;
    bool isSquareAttacked(const Board& b, int row, int col, Color defender) const;

    mutable Board board;
    Color currentTurn;
    std::pair<int, int> whiteKingPos;
    std::pair<int, int> blackKingPos;

    std::optional<std::pair<int, int>> enPassantTarget;

    void addCastlingMoves(int row, int col, std::vector<Move>& moves) const;
    void addEnPassantMoves(int row, int col, std::vector<Move>& moves) const;
    void addPromotionMoves(std::vector<Move>& moves) const;

    //aktív bábúk koorddinátái
    std::vector<std::pair<int, int>> whitePieceSquares;
    std::vector<std::pair<int, int>> blackPieceSquares;



};
