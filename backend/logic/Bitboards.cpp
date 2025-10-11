#include "Bitboards.h"


// segédfüggvények
inline int squareIndex(int row, int col) {
    return row * 8 + col; // 0..63
}

uint64_t coordsToBitboard(const std::vector<std::pair<int, int>>& coords) {
    uint64_t bb = 0ULL;
    for (auto [r, c] : coords) {
        int sq = squareIndex(r, c);
        bb |= (1ULL << sq);
    }
    return bb;
};


void Bitboard::resetEmpty() {
    whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0ULL;
    blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0ULL;
    whitePieces = blackPieces = allPieces = 0ULL;
}

void Bitboard::reset() {

    resetEmpty();

    // kezdőállás
    whitePawns = 0x000000000000FF00ULL; // 2. sor
    whiteRooks = 0x0000000000000081ULL; // a1, h1
    whiteKnights = 0x0000000000000042ULL; // b1, g1
    whiteBishops = 0x0000000000000024ULL; // c1, f1
    whiteQueens = 0x0000000000000008ULL; // d1
    whiteKing = 0x0000000000000010ULL; // e1

    blackPawns = 0x00FF000000000000ULL; // 7. sor
    blackRooks = 0x8100000000000000ULL; // a8, h8
    blackKnights = 0x4200000000000000ULL; // b8, g8
    blackBishops = 0x2400000000000000ULL; // c8, f8
    blackQueens = 0x0800000000000000ULL; // d8
    blackKing = 0x1000000000000000ULL; // e8

    // összesítés
    whitePieces = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
    blackPieces = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
    allPieces = whitePieces | blackPieces;
}

void Bitboard::load(const std::string& str) {
    resetEmpty();

    std::istringstream iss(str);
    std::string token;
    std::vector<std::vector<char>> board(8, std::vector<char>(8, '.'));

    // beolvassuk soronként
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            iss >> token;
            if (!token.empty()) {
                board[r][c] = token[0];
            }
        }
    }

    // feltöltés
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            char ch = board[r][c];
            if (ch == '.') continue;

            int sq = squareIndex(r, c);
            uint64_t mask = (1ULL << sq);

            switch (ch) {
                // fekete
            case 'p': blackPawns |= mask; break;
            case 'n': blackKnights |= mask; break;
            case 'b': blackBishops |= mask; break;
            case 'r': blackRooks |= mask; break;
            case 'q': blackQueens |= mask; break;
            case 'k': blackKing |= mask; break;
                // fehér
            case 'P': whitePawns |= mask; break;
            case 'N': whiteKnights |= mask; break;
            case 'B': whiteBishops |= mask; break;
            case 'R': whiteRooks |= mask; break;
            case 'Q': whiteQueens |= mask; break;
            case 'K': whiteKing |= mask; break;
            }
        }
    }

    // összesítés
    whitePieces = whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
    blackPieces = blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
    allPieces = whitePieces | blackPieces;
}

std::string Bitboard::debugString() const {
    std::ostringstream oss;
    oss << "Bitboard state:\n";
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int sq = r * 8 + c;
            char ch = '.';

            if (whitePawns & (1ULL << sq)) ch = 'p';
            else if (whiteKnights & (1ULL << sq)) ch = 'n';
            else if (whiteBishops & (1ULL << sq)) ch = 'b';
            else if (whiteRooks & (1ULL << sq)) ch = 'r';
            else if (whiteQueens & (1ULL << sq)) ch = 'q';
            else if (whiteKing & (1ULL << sq)) ch = 'k';

            else if (blackPawns & (1ULL << sq)) ch = 'P';
            else if (blackKnights & (1ULL << sq)) ch = 'N';
            else if (blackBishops & (1ULL << sq)) ch = 'B';
            else if (blackRooks & (1ULL << sq)) ch = 'R';
            else if (blackQueens & (1ULL << sq)) ch = 'Q';
            else if (blackKing & (1ULL << sq)) ch = 'K';

            oss << ch << " ";
        }
        oss << "\n";
    }
    return oss.str();
}