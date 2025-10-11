#pragma once
#include "Board.h"
#include "Piece.h"
#include "Move.h"
#include "Bitboards.h"
#include <optional>
#include <sstream>
#include <string>

#define EMPTY 0



class Game {
public:
    Game();
    void resetBoard();
    void loadPosition(const std::string& position, Color turn);

    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);

	void updateTurn();

    Color getCurrentTurn() const;
    const Board& getBoard() const;
	const Bitboard& getBitboard() const;
    std::vector<Move> getLegalMoves(int fromRow, int fromCol) const;

    std::string debugBoardString() const;
    std::string debugBitboardString() const;

	bool isCheckmate() const;
	bool isStalemate() const;
    bool isSquareAttacked(const Bitboard& bb, int row, int col, Color defender) const;

    bool checkMate = false;
    bool staleMate = false;

private:
    bool wouldBeInCheckAfterMove(const Move& move) const;

    void applyMove(Bitboard& bb, const Move& m) const;

    mutable Board board;
    Color currentTurn;

    std::optional<std::pair<int, int>> enPassantTarget;

    void addCastlingMoves(int row, int col, std::vector<Move>& moves) const;
    void addEnPassantMoves(int row, int col, std::vector<Move>& moves) const;
    void addPromotionMoves(std::vector<Move>& moves) const;

	Bitboard bitboard;
    
};
