#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

struct Bitboard {
    uint64_t whitePawns = 0ULL;
    uint64_t whiteKnights = 0ULL;
    uint64_t whiteBishops = 0ULL;
    uint64_t whiteRooks = 0ULL;
    uint64_t whiteQueens = 0ULL;
    uint64_t whiteKing = 0ULL;

    uint64_t blackPawns = 0ULL;
    uint64_t blackKnights = 0ULL;
    uint64_t blackBishops = 0ULL;
    uint64_t blackRooks = 0ULL;
    uint64_t blackQueens = 0ULL;
    uint64_t blackKing = 0ULL;

    uint64_t whitePieces = 0ULL;
    uint64_t blackPieces = 0ULL;
    uint64_t allPieces = 0ULL;

    void resetEmpty();
    void reset();
    void load(const std::string& str);
};
