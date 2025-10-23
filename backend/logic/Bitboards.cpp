#include "Bitboards.h"
#include "Move.h"


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
    blackPawns = 0x000000000000FF00ULL; // 2. sor
    blackRooks = 0x0000000000000081ULL; // a1, h1
    blackKnights = 0x0000000000000042ULL; // b1, g1
    blackBishops = 0x0000000000000024ULL; // c1, f1
    blackQueens = 0x0000000000000008ULL; // d1
    blackKing = 0x0000000000000010ULL; // e1

    whitePawns = 0x00FF000000000000ULL; // 7. sor
    whiteRooks = 0x8100000000000000ULL; // a8, h8
    whiteKnights = 0x4200000000000000ULL; // b8, g8
    whiteBishops = 0x2400000000000000ULL; // c8, f8
    whiteQueens = 0x0800000000000000ULL; // d8
    whiteKing = 0x1000000000000000ULL; // e8

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

            if (whitePawns & (1ULL << sq)) ch = 'P';
            else if (whiteKnights & (1ULL << sq)) ch = 'N';
            else if (whiteBishops & (1ULL << sq)) ch = 'B';
            else if (whiteRooks & (1ULL << sq)) ch = 'R';
            else if (whiteQueens & (1ULL << sq)) ch = 'Q';
            else if (whiteKing & (1ULL << sq)) ch = 'K';

            else if (blackPawns & (1ULL << sq)) ch = 'p';
            else if (blackKnights & (1ULL << sq)) ch = 'n';
            else if (blackBishops & (1ULL << sq)) ch = 'b';
            else if (blackRooks & (1ULL << sq)) ch = 'r';
            else if (blackQueens & (1ULL << sq)) ch = 'q';
            else if (blackKing & (1ULL << sq)) ch = 'k';

            oss << ch << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

void Bitboard::applyMove(const struct Move& m) {
    int fromSq = m.fromRow * 8 + m.fromCol;
    int toSq = m.toRow * 8 + m.toCol;

    uint64_t fromMask = ~(1ULL << fromSq);
    uint64_t toMask = (1ULL << toSq);

    // --- Mozgó bábu színének és típusának meghatározása ---
    Color movingColor;
    PieceType pieceType = PieceType::None;
    uint64_t mask = (1ULL << fromSq);

    if ( whitePawns & mask) { movingColor = Color::White; pieceType = PieceType::Pawn; }
    else if ( whiteKnights & mask) { movingColor = Color::White; pieceType = PieceType::Knight; }
    else if ( whiteBishops & mask) { movingColor = Color::White; pieceType = PieceType::Bishop; }
    else if ( whiteRooks & mask) { movingColor = Color::White; pieceType = PieceType::Rook; }
    else if ( whiteQueens & mask) { movingColor = Color::White; pieceType = PieceType::Queen; }
    else if ( whiteKing & mask) { movingColor = Color::White; pieceType = PieceType::King; }
    else if ( blackPawns & mask) { movingColor = Color::Black; pieceType = PieceType::Pawn; }
    else if ( blackKnights & mask) { movingColor = Color::Black; pieceType = PieceType::Knight; }
    else if ( blackBishops & mask) { movingColor = Color::Black; pieceType = PieceType::Bishop; }
    else if ( blackRooks & mask) { movingColor = Color::Black; pieceType = PieceType::Rook; }
    else if ( blackQueens & mask) { movingColor = Color::Black; pieceType = PieceType::Queen; }
    else if ( blackKing & mask) { movingColor = Color::Black; pieceType = PieceType::King; }

    // --- Capture (normál vagy EP) ---
    if (m.isEnPassant) {
        int capSq = m.fromRow * 8 + m.toCol;
        uint64_t capMask = ~(1ULL << capSq);
        if (movingColor == Color::White) {
             blackPawns &= capMask;
             blackPieces &= capMask;
        }
        else {
             whitePawns &= capMask;
             whitePieces &= capMask;
        }
    }
    else {
        uint64_t toMaskInv = ~toMask;
        if ( allPieces & toMask) {
            if (movingColor == Color::White) {
                 blackPawns &= toMaskInv;
                 blackKnights &= toMaskInv;
                 blackBishops &= toMaskInv;
                 blackRooks &= toMaskInv;
                 blackQueens &= toMaskInv;
                 blackKing &= toMaskInv;
                 blackPieces &= toMaskInv;
            }
            else {
                 whitePawns &= toMaskInv;
                 whiteKnights &= toMaskInv;
                 whiteBishops &= toMaskInv;
                 whiteRooks &= toMaskInv;
                 whiteQueens &= toMaskInv;
                 whiteKing &= toMaskInv;
                 whitePieces &= toMaskInv;
            }
        }
    }

    // --- Castle speciális ---
    if (m.isCastle) {
        int dir = (m.toCol > m.fromCol) ? 1 : -1;
        int rookCol = (dir == 1) ? 7 : 0;
        int rookTargetCol = m.fromCol + dir;

        int rookFromSq = m.fromRow * 8 + rookCol;
        int rookToSq = m.fromRow * 8 + rookTargetCol;

        uint64_t rookFromMask = ~(1ULL << rookFromSq);
        uint64_t rookToMask = (1ULL << rookToSq);

        if (movingColor == Color::White) {
             whiteRooks &= rookFromMask;
             whiteRooks |= rookToMask;
             whitePieces &= rookFromMask;
             whitePieces |= rookToMask;
        }
        else {
             blackRooks &= rookFromMask;
             blackRooks |= rookToMask;
             blackPieces &= rookFromMask;
             blackPieces |= rookToMask;
        }
    }

    // --- Mozgatás a típus bitboardban ---
    if (movingColor == Color::White) {
        switch (pieceType) {
        case PieceType::Pawn:    whitePawns &= fromMask;  whitePawns |= toMask; break;
        case PieceType::Knight:  whiteKnights &= fromMask;  whiteKnights |= toMask; break;
        case PieceType::Bishop:  whiteBishops &= fromMask;  whiteBishops |= toMask; break;
        case PieceType::Rook:    whiteRooks &= fromMask;  whiteRooks |= toMask; break;
        case PieceType::Queen:   whiteQueens &= fromMask;  whiteQueens |= toMask; break;
        case PieceType::King:    whiteKing &= fromMask;  whiteKing |= toMask; break;
        default: break;
        }
         whitePieces &= fromMask;
         whitePieces |= toMask;
    }
    else {
        switch (pieceType) {
        case PieceType::Pawn:    blackPawns &= fromMask;  blackPawns |= toMask; break;
        case PieceType::Knight:  blackKnights &= fromMask;  blackKnights |= toMask; break;
        case PieceType::Bishop:  blackBishops &= fromMask;  blackBishops |= toMask; break;
        case PieceType::Rook:    blackRooks &= fromMask;  blackRooks |= toMask; break;
        case PieceType::Queen:   blackQueens &= fromMask;  blackQueens |= toMask; break;
        case PieceType::King:    blackKing &= fromMask;  blackKing |= toMask; break;
        default: break;
        }
         blackPieces &= fromMask;
         blackPieces |= toMask;
    }

    // --- Promóció ---
    if (m.isPromotion()) {
        uint64_t maskTo = (1ULL << toSq);
        if (movingColor == Color::White) {
             whitePawns &= ~maskTo;
             whiteQueens |= maskTo;
        }
        else {
             blackPawns &= ~maskTo;
             blackQueens |= maskTo;
        }
    }

    // --- allPieces frissítés ---
     allPieces =  whitePieces |  blackPieces;
}