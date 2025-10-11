#include "Game.h"
#include "pieces/Piece.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include "Board.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

// ---------------- Implementáció ----------------
Game::Game() {
    resetBoard();
}

bool debugMode = true;

const Bitboard& Game::getBitboard() const {
    return bitboard;
}   

void Game::loadPosition(const std::string& position, Color turn) {
    board.clear();

    checkMate = false;
    staleMate= false;
    enPassantTarget.reset();

    std::istringstream iss(position);
    std::string line;
    int row = 0;

    while (std::getline(iss, line)) {
        if (line.empty()) continue;

        std::istringstream lineStream(line);
        std::string token;
        int col = 0;

        while (lineStream >> token) {
            char c = token[0];
            if (c == '.') {
                // üres mező
            }
            else {
                Color color = isupper(c) ? Color::White : Color::Black;
                char pieceChar = static_cast<char>(tolower(c));
                std::shared_ptr<Piece> piece;

                switch (pieceChar) {
                case 'k': piece = std::make_shared<King>(color); break;
                case 'q': piece = std::make_shared<Queen>(color); break;
                case 'r': piece = std::make_shared<Rook>(color); break;
                case 'n': piece = std::make_shared<Knight>(color); break;
                case 'b': piece = std::make_shared<Bishop>(color); break;
                case 'p': piece = std::make_shared<Pawn>(color); break;
                default: break;
                }

                if (piece) {
                    board.setPiece(row, col, piece);
                }
            }
            ++col;
        }
        ++row;
    }

    bitboard.load(position);

    currentTurn = turn;
}

void Game::resetBoard() {
    board.clear();
    
    // --- Fekete bábuk felül ---
    board.setPiece(0, 4, std::make_shared<King>(Color::Black));

    board.setPiece(0, 3, std::make_shared<Queen>(Color::Black));

    board.setPiece(0, 0, std::make_shared<Rook>(Color::Black));
    board.setPiece(0, 7, std::make_shared<Rook>(Color::Black));

    board.setPiece(0, 1, std::make_shared<Knight>(Color::Black));
    board.setPiece(0, 6, std::make_shared<Knight>(Color::Black));

    board.setPiece(0, 2, std::make_shared<Bishop>(Color::Black));
    board.setPiece(0, 5, std::make_shared<Bishop>(Color::Black));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(1, col, std::make_shared<Pawn>(Color::Black));
    }

    // --- Fehér bábuk alul ---
    board.setPiece(7, 4, std::make_shared<King>(Color::White));

    board.setPiece(7, 3, std::make_shared<Queen>(Color::White));

    board.setPiece(7, 0, std::make_shared<Rook>(Color::White));
    board.setPiece(7, 7, std::make_shared<Rook>(Color::White));

    board.setPiece(7, 1, std::make_shared<Knight>(Color::White));
    board.setPiece(7, 6, std::make_shared<Knight>(Color::White));

    board.setPiece(7, 2, std::make_shared<Bishop>(Color::White));
    board.setPiece(7, 5, std::make_shared<Bishop>(Color::White));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(6, col, std::make_shared<Pawn>(Color::White));
    }

    //bitboard alaphelyzet
    bitboard.reset();

    // Fehér kezd
    currentTurn = Color::White;

	//checkmate, stalemate reset
	checkMate = false;
	staleMate = false;

    // En passant reset
    enPassantTarget.reset();
}


bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;
    if (piece->getColor() != currentTurn) return false;
    if (checkMate || staleMate) return false; // játék vége után nem lehet lépni

    // 1. Legális lépések lekérése
    auto legalMoves = getLegalMoves(fromRow, fromCol);

    // 2. Keresés
    Move chosenMove(fromRow, fromCol, toRow, toCol);
    bool found = false;
    for (const auto& m : legalMoves) {
        if (m.toRow == toRow && m.toCol == toCol) {
            chosenMove = m;
            found = true;
            break;
        }
    }
    if (!found) return false;

    // 3. Board frissítése
    if (chosenMove.isCastle) {
        int dir = (toCol > fromCol) ? 1 : -1;
        int rookCol = (dir == 1) ? 7 : 0;
        int rookTargetCol = fromCol + dir;
        board.movePiece(fromRow, fromCol, toRow, toCol);             // király
        board.movePiece(fromRow, rookCol, fromRow, rookTargetCol);   // bástya
    }
    else if (chosenMove.isEnPassant) {
        board.movePiece(fromRow, fromCol, toRow, toCol);
        // levett gyalog törlése a tábláról
        board.setPiece(fromRow, toCol, nullptr);
    }
    else {
        board.movePiece(fromRow, fromCol, toRow, toCol);
    }

    // 4. Bitboard frissítése az applyMove segítségével
    applyMove(bitboard, chosenMove);

    // 5. Promóció (fixen vezér)
    auto movedPiece = board.getPiece(toRow, toCol);
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        int promotionRow = (movedPiece->getColor() == Color::White) ? 0 : 7;
        if (toRow == promotionRow) {
            auto newQueen = std::make_shared<Queen>(movedPiece->getColor());
            board.setPiece(toRow, toCol, newQueen);
            movedPiece = newQueen;
            // Bitboard promóciós részét az applyMove már kezeli, ha a Move‑ban be van állítva a promotion flag
        }
    }

    // 6. Flag-ek frissítése
    if (movedPiece) {
        if (movedPiece->getType() == PieceType::King) {
            static_cast<King*>(movedPiece.get())->setMoved(true);
        }
        else if (movedPiece->getType() == PieceType::Rook) {
            static_cast<Rook*>(movedPiece.get())->setMoved(true);
        }
    }

    // 7. En passant target
    enPassantTarget.reset();
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        if (std::abs(toRow - fromRow) == 2) {
            int midRow = (fromRow + toRow) / 2;
            enPassantTarget = { midRow, toCol };
        }
    }

    // 8. Játék vége ellenőrzés
    if (isCheckmate()) {
        checkMate = true;
    }
    else if (isStalemate()) {
        staleMate = true;
    }
    else {
        updateTurn();
    }

    return true;
}

void Game::updateTurn() {
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
}


std::vector<Move> Game::getLegalMoves(int fromRow, int fromCol) const {
    std::vector<Move> legalMoves;
    auto piece = board.getPiece(fromRow, fromCol);

    // 1. pszeudo-lépések a bábutól
    auto pseudoMoves = piece->getPossibleMoves(board, fromRow, fromCol);

    // speciális lépések csak ha kell
    switch (piece->getType()) {
    case PieceType::King:
        addCastlingMoves(fromRow, fromCol, pseudoMoves);
        break;
    case PieceType::Pawn:
        addEnPassantMoves(fromRow, fromCol, pseudoMoves);
        addPromotionMoves(pseudoMoves);
        break;
    default:
        break;
    }

    // 2. szűrés sakk ellen
    
    for (const auto& move : pseudoMoves) {
        if (move.isCastle) {
			legalMoves.push_back(move); // sáncolásnál már ellenőriztük a sakkot (neki spéci szabály van)
        }
        else{
			std::cout << "Ellenőrzés alatt lévő lépés: (" << move.fromRow << "," << move.fromCol << ") -> (" << move.toRow << "," << move.toCol << ")\n";
            if (!wouldBeInCheckAfterMove(move)) {
                legalMoves.push_back(move);
			}
            else {
                std::cout << "Lépés sakkot eredményezne, kihagyva.\n";
            }
        }
    }

    return legalMoves;
}

void Game::addCastlingMoves(int row, int col, std::vector<Move>& moves) const {
    auto piece = board.getPiece(row, col);
    if (!piece || piece->getType() != PieceType::King) return;

    auto king = static_cast<King*>(piece.get());
    if (king->hasMoved()) return;

    // két irány: rövid (jobbra), hosszú (balra)
    for (int dir : {1, -1}) {
        int rookCol = (dir == 1) ? 7 : 0;
        auto rook = board.getPiece(row, rookCol);
        if (!rook || rook->getType() != PieceType::Rook) continue;
        if (static_cast<Rook*>(rook.get())->hasMoved()) continue;

        // út tisztaság ellenőrzése
        bool pathClear = true;
        for (int c = col + dir; c != rookCol; c += dir) {
            if (board.getPiece(row, c)) { pathClear = false; break; }
        }
        if (!pathClear) continue;

        // sakkmentes átlépés
        bool safe = true;
        for (int step = 0; step <= 2; step++) {
            int checkCol = col + step * dir;
			Move move(row, col, row, checkCol);
            if (wouldBeInCheckAfterMove(move)) {
                safe = false; break;
            }
        }
        if (!safe) continue;

        // ha minden oké, hozzáadjuk
        Move castleMove(row, col, row, col + 2 * dir);
        castleMove.isCastle = true;
        moves.push_back(castleMove);
    }
}

void Game::addEnPassantMoves(int row, int col, std::vector<Move>& moves) const {
    auto piece = board.getPiece(row, col);
    if (!piece || piece->getType() != PieceType::Pawn) return;

    if (!enPassantTarget) return; // nincs EP lehetőség

    int targetRow = enPassantTarget->first;
    int targetCol = enPassantTarget->second;

    // csak akkor érvényes, ha a gyalog mellette áll
    if (row == ((piece->getColor() == Color::White) ? 3 : 4) &&
        std::abs(col - targetCol) == 1) {
        Move epMove(row, col, targetRow, targetCol);
        epMove.isEnPassant = true;
        moves.push_back(epMove);
    }
}

void Game::addPromotionMoves(std::vector<Move>& moves) const {
    std::vector<Move> extra;
    for (auto& m : moves) {
        auto piece = board.getPiece(m.fromRow, m.fromCol);
        if (piece && piece->getType() == PieceType::Pawn) {
            int promotionRow = (piece->getColor() == Color::White) ? 0 : 7;
            if (m.toRow == promotionRow) {
                Move promo = m;
                promo.promotion = PieceType::Queen; // fixen vezér
                extra.push_back(promo);
            }
        }
    }

    // töröljük az eredeti gyaloglépést, ami a túloldalra ment
    moves.erase(std::remove_if(moves.begin(), moves.end(),
        [&](const Move& m) {
            auto piece = board.getPiece(m.fromRow, m.fromCol);
            return piece && piece->getType() == PieceType::Pawn &&
                (m.toRow == 0 || m.toRow == 7);
        }), moves.end());

    // hozzáadjuk a promóciós verziót
    moves.insert(moves.end(), extra.begin(), extra.end());

}

void Game::applyMove(Bitboard& bb, const Move& m) const {
    int fromSq = m.fromRow * 8 + m.fromCol;
    int toSq = m.toRow * 8 + m.toCol;

    uint64_t fromMask = ~(1ULL << fromSq);
    uint64_t toMask = (1ULL << toSq);

    // --- Mozgó bábu színének és típusának meghatározása ---
    Color movingColor;
    PieceType pieceType = PieceType::None;
    uint64_t mask = (1ULL << fromSq);

    if (bb.whitePawns & mask) { movingColor = Color::White; pieceType = PieceType::Pawn; }
    else if (bb.whiteKnights & mask) { movingColor = Color::White; pieceType = PieceType::Knight; }
    else if (bb.whiteBishops & mask) { movingColor = Color::White; pieceType = PieceType::Bishop; }
    else if (bb.whiteRooks & mask) { movingColor = Color::White; pieceType = PieceType::Rook; }
    else if (bb.whiteQueens & mask) { movingColor = Color::White; pieceType = PieceType::Queen; }
    else if (bb.whiteKing & mask) { movingColor = Color::White; pieceType = PieceType::King; }
    else if (bb.blackPawns & mask) { movingColor = Color::Black; pieceType = PieceType::Pawn; }
    else if (bb.blackKnights & mask) { movingColor = Color::Black; pieceType = PieceType::Knight; }
    else if (bb.blackBishops & mask) { movingColor = Color::Black; pieceType = PieceType::Bishop; }
    else if (bb.blackRooks & mask) { movingColor = Color::Black; pieceType = PieceType::Rook; }
    else if (bb.blackQueens & mask) { movingColor = Color::Black; pieceType = PieceType::Queen; }
    else if (bb.blackKing & mask) { movingColor = Color::Black; pieceType = PieceType::King; }

    // --- Capture (normál vagy EP) ---
    if (m.isEnPassant) {
        int capSq = m.fromRow * 8 + m.toCol;
        uint64_t capMask = ~(1ULL << capSq);
        if (movingColor == Color::White) {
            bb.blackPawns &= capMask;
            bb.blackPieces &= capMask;
        }
        else {
            bb.whitePawns &= capMask;
            bb.whitePieces &= capMask;
        }
    }
    else {
        uint64_t toMaskInv = ~toMask;
        if (bb.allPieces & toMask) {
            if (movingColor == Color::White) {
                bb.blackPawns &= toMaskInv;
                bb.blackKnights &= toMaskInv;
                bb.blackBishops &= toMaskInv;
                bb.blackRooks &= toMaskInv;
                bb.blackQueens &= toMaskInv;
                bb.blackKing &= toMaskInv;
                bb.blackPieces &= toMaskInv;
            }
            else {
                bb.whitePawns &= toMaskInv;
                bb.whiteKnights &= toMaskInv;
                bb.whiteBishops &= toMaskInv;
                bb.whiteRooks &= toMaskInv;
                bb.whiteQueens &= toMaskInv;
                bb.whiteKing &= toMaskInv;
                bb.whitePieces &= toMaskInv;
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
            bb.whiteRooks &= rookFromMask;
            bb.whiteRooks |= rookToMask;
            bb.whitePieces &= rookFromMask;
            bb.whitePieces |= rookToMask;
        }
        else {
            bb.blackRooks &= rookFromMask;
            bb.blackRooks |= rookToMask;
            bb.blackPieces &= rookFromMask;
            bb.blackPieces |= rookToMask;
        }
    }

    // --- Mozgatás a típus bitboardban ---
    if (movingColor == Color::White) {
        switch (pieceType) {
        case PieceType::Pawn:   bb.whitePawns &= fromMask; bb.whitePawns |= toMask; break;
        case PieceType::Knight: bb.whiteKnights &= fromMask; bb.whiteKnights |= toMask; break;
        case PieceType::Bishop: bb.whiteBishops &= fromMask; bb.whiteBishops |= toMask; break;
        case PieceType::Rook:   bb.whiteRooks &= fromMask; bb.whiteRooks |= toMask; break;
        case PieceType::Queen:  bb.whiteQueens &= fromMask; bb.whiteQueens |= toMask; break;
        case PieceType::King:   bb.whiteKing &= fromMask; bb.whiteKing |= toMask; break;
        default: break;
        }
        bb.whitePieces &= fromMask;
        bb.whitePieces |= toMask;
    }
    else {
        switch (pieceType) {
        case PieceType::Pawn:   bb.blackPawns &= fromMask; bb.blackPawns |= toMask; break;
        case PieceType::Knight: bb.blackKnights &= fromMask; bb.blackKnights |= toMask; break;
        case PieceType::Bishop: bb.blackBishops &= fromMask; bb.blackBishops |= toMask; break;
        case PieceType::Rook:   bb.blackRooks &= fromMask; bb.blackRooks |= toMask; break;
        case PieceType::Queen:  bb.blackQueens &= fromMask; bb.blackQueens |= toMask; break;
        case PieceType::King:   bb.blackKing &= fromMask; bb.blackKing |= toMask; break;
        default: break;
        }
        bb.blackPieces &= fromMask;
        bb.blackPieces |= toMask;
    }

    // --- Promóció ---
    if (m.isPromotion()) {
        uint64_t maskTo = (1ULL << toSq);
        if (movingColor == Color::White) {
            bb.whitePawns &= ~maskTo;
            bb.whiteQueens |= maskTo;
        }
        else {
            bb.blackPawns &= ~maskTo;
            bb.blackQueens |= maskTo;
        }
    }

    // --- allPieces frissítés ---
    bb.allPieces = bb.whitePieces | bb.blackPieces;
}

bool Game::wouldBeInCheckAfterMove(const Move& move) const {
    // 1) Temp bitboard az aktuálisból
    Bitboard temp = bitboard;

    // 2) Lépő szín meghatározása CSAK bitboardból (nem a Board-ból)
    const int fromSq = move.fromRow * 8 + move.fromCol;
    const uint64_t fromBit = (1ULL << fromSq);
    Color movingColor =
        (bitboard.whitePieces & fromBit) ? Color::White : Color::Black;

    // 3) Lépés alkalmazása a temp bitboardon
    applyMove(temp, move);

    // 4) Király pozíció a lépés UTÁNI állapotban (temp)
    int kingSq;
    if (movingColor == Color::White) {
        if (temp.whiteKing == 0ULL) return false; // hibás állapot → tekintsük sakkban
        kingSq = __builtin_ctzll(temp.whiteKing);
    }
    else {
        if (temp.blackKing == 0ULL) return false;
        kingSq = __builtin_ctzll(temp.blackKing);
    }
    const int kingRow = kingSq / 8;
    const int kingCol = kingSq % 8;

    // 5) A lépő fél királyát az ellenfél támadja-e?
    // isSquareAttacked 'defender' paramétere a király színe, azaz movingColor
    return isSquareAttacked(temp, kingRow, kingCol, movingColor);
}



bool Game::isSquareAttacked(const Bitboard& bb, int row, int col, Color defender) const {
    int sq = row * 8 + col;
    uint64_t sqMask = (1ULL << sq);

    Color attacker = (defender == Color::White) ? Color::Black : Color::White;

    // --- Pawn attacks ---
    if (attacker == Color::White) {
        // fehér gyalog támadási maszk (felfelé bal/jobb)
        uint64_t pawnAttacks = 0ULL;
        if (col > 0 && row > 0) pawnAttacks |= (1ULL << ((row - 1) * 8 + (col - 1)));
        if (col < 7 && row > 0) pawnAttacks |= (1ULL << ((row - 1) * 8 + (col + 1)));
        if (pawnAttacks & bb.whitePawns) return true;
    }
    else {
        // fekete gyalog támadási maszk (lefelé bal/jobb)
        uint64_t pawnAttacks = 0ULL;
        if (col > 0 && row < 7) pawnAttacks |= (1ULL << ((row + 1) * 8 + (col - 1)));
        if (col < 7 && row < 7) pawnAttacks |= (1ULL << ((row + 1) * 8 + (col + 1)));
        if (pawnAttacks & bb.blackPawns) return true;
    }

    // --- Knight attacks ---
    static const int knightOffsets[8][2] = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},
        {1,-2},{1,2},{2,-1},{2,1}
    };
    for (auto& off : knightOffsets) {
        int r = row + off[0], c = col + off[1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int nsq = r * 8 + c;
            if (attacker == Color::White && (bb.whiteKnights & (1ULL << nsq))) return true;
            if (attacker == Color::Black && (bb.blackKnights & (1ULL << nsq))) return true;
        }
    }

    // --- Bishop / Queen diagonals ---
    static const int diagDirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    for (auto& d : diagDirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int nsq = r * 8 + c;
            uint64_t mask = (1ULL << nsq);
            if (bb.allPieces & mask) {
                if (attacker == Color::White &&
                    (bb.whiteBishops & mask || bb.whiteQueens & mask)) return true;
                if (attacker == Color::Black &&
                    (bb.blackBishops & mask || bb.blackQueens & mask)) return true;
                break; // ütközés
            }
            r += d[0]; c += d[1];
        }
    }

    // --- Rook / Queen lines ---
    static const int lineDirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& d : lineDirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int nsq = r * 8 + c;
            uint64_t mask = (1ULL << nsq);
            if (bb.allPieces & mask) {
                if (attacker == Color::White &&
                    (bb.whiteRooks & mask || bb.whiteQueens & mask)) return true;
                if (attacker == Color::Black &&
                    (bb.blackRooks & mask || bb.blackQueens & mask)) return true;
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // --- King ---
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr, c = col + dc;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                int nsq = r * 8 + c;
                if (attacker == Color::White && (bb.whiteKing & (1ULL << nsq))) return true;
                if (attacker == Color::Black && (bb.blackKing & (1ULL << nsq))) return true;
            }
        }
    }

    return false;
}



Color Game::getCurrentTurn() const {
	return currentTurn;
}


const Board& Game::getBoard() const {
	return board;
}

bool Game::isCheckmate() const {
    // 1. Ki a védekező fél?
    Color defender = currentTurn;

    if (debugMode) {
		cout << "Ellenőrzés alatt lévő fél: " << ((defender == Color::White) ? "Fehér" : "Fekete") << endl;
    }
    // 2. Király pozíció
    int kingSq;
    if (defender == Color::White) {
        if (bitboard.whiteKing == 0ULL) return false; // nincs király → hibás állapot
        kingSq = __builtin_ctzll(bitboard.whiteKing);
    }
    else {
        if (bitboard.blackKing == 0ULL) return false;
        kingSq = __builtin_ctzll(bitboard.blackKing);
    }
    int kingRow = kingSq / 8;
    int kingCol = kingSq % 8;

    // 3. Sakkban van‑e?
    bool inCheck = isSquareAttacked(bitboard, kingRow, kingCol, defender);

    if (debugMode) {
		cout << "Király pozíció: (" << kingRow << "," << kingCol << ")\n";
		cout << "Sakkban van-e? " << (inCheck ? "Igen" : "Nem") << endl;
    }
    if (!inCheck) return false;

    // 4. Van‑e legális lépés?
    // végigmegyünk a védekező fél összes bábuján
    uint64_t pieces = (defender == Color::White) ? bitboard.whitePieces : bitboard.blackPieces;
    while (pieces) {
        int sq = __builtin_ctzll(pieces); // legalsó beállított bit indexe
        pieces &= (pieces - 1);           // töröljük a feldolgozott bitet

        int r = sq / 8;
        int c = sq % 8;

        auto moves = getLegalMoves(r, c);



        if (!moves.empty()) {
            return false;
			cout << "Van legális lépés a (" << r << "," << c << ") bábúval.\n";
        }
    }

    return true; // sakkban van és nincs legális lépés → matt
}

bool Game::isStalemate() const {
    // 1. Ki a védekező fél?
    Color defender = currentTurn;

    // 2. Király pozíció
    int kingSq;
    if (defender == Color::White) {
        if (bitboard.whiteKing == 0ULL) return false;
        kingSq = __builtin_ctzll(bitboard.whiteKing);
    }
    else {
        if (bitboard.blackKing == 0ULL) return false;
        kingSq = __builtin_ctzll(bitboard.blackKing);
    }
    int kingRow = kingSq / 8;
    int kingCol = kingSq % 8;

    // 3. Ha sakkban van, nem lehet patt
    bool inCheck = isSquareAttacked(bitboard, kingRow, kingCol, defender);
    if (inCheck) return false;

    // 4. Van‑e legális lépés?
    uint64_t pieces = (defender == Color::White) ? bitboard.whitePieces : bitboard.blackPieces;
    while (pieces) {
        int sq = __builtin_ctzll(pieces);
        pieces &= (pieces - 1);

        int r = sq / 8;
        int c = sq % 8;

        auto moves = getLegalMoves(r, c);
        if (!moves.empty()) return false;
    }

    return true; // nincs sakk és nincs legális lépés → patt
}


std::string Game::debugBoardString() const {
    std::ostringstream oss;
    oss << "Board state: \n";
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto piece = board.getPiece(r, c);
            if (!piece) {
                oss << ".";
            }
            else {
                char ch = '?';
                switch (piece->getType()) {
                case PieceType::King:   ch = 'K'; break;
                case PieceType::Queen:  ch = 'Q'; break;
                case PieceType::Rook:   ch = 'R'; break;
                case PieceType::Bishop: ch = 'B'; break;
                case PieceType::Knight: ch = 'N'; break;
                case PieceType::Pawn:   ch = 'P'; break;
                }
                if (piece->getColor() == Color::Black)
                    ch = std::tolower(ch);
                oss << ch;
            }
            oss << " ";
        }
        oss << "\n";
    }
    return oss.str();
}

std::string Game::debugBitboardString() const {
    std::ostringstream oss;
    oss << "Bitboard state:\n";

    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int sq = r * 8 + c;
            char ch = '.'; // üres mező

            // White pieces
            if (bitboard.whitePawns & (1ULL << sq)) ch = 'p';
            else if (bitboard.whiteKnights & (1ULL << sq)) ch = 'n';
            else if (bitboard.whiteBishops & (1ULL << sq)) ch = 'b';
            else if (bitboard.whiteRooks & (1ULL << sq)) ch = 'r';
            else if (bitboard.whiteQueens & (1ULL << sq)) ch = 'q';
            else if (bitboard.whiteKing & (1ULL << sq)) ch = 'k';

            // Black pieces (kisbetűvel)
            else if (bitboard.blackPawns & (1ULL << sq)) ch = 'P';
            else if (bitboard.blackKnights & (1ULL << sq)) ch = 'N';
            else if (bitboard.blackBishops & (1ULL << sq)) ch = 'B';
            else if (bitboard.blackRooks & (1ULL << sq)) ch = 'R';
            else if (bitboard.blackQueens & (1ULL << sq)) ch = 'Q';
            else if (bitboard.blackKing & (1ULL << sq)) ch = 'K';

            oss << ch << " ";
        }
        oss << "\n";
    }

    return oss.str();
}