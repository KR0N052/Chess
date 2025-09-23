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


// ---------------- Implementáció ----------------
Game::Game() {
    resetBoard();
}


void Game::resetBoard() {
    board.clear();
    whitePieces.clear();
    blackPieces.clear();

    // --- Fekete bábuk felül ---
    auto bKing = std::make_shared<King>(Color::Black);
    board.setPiece(0, 4, bKing);
    blackPieces.push_back(bKing); // király mindig első

    auto bQueen = std::make_shared<Queen>(Color::Black);
    board.setPiece(0, 3, bQueen);
    blackPieces.push_back(bQueen);

    auto bRookA = std::make_shared<Rook>(Color::Black);
    auto bRookH = std::make_shared<Rook>(Color::Black);
    board.setPiece(0, 0, bRookA);
    board.setPiece(0, 7, bRookH);
    blackPieces.push_back(bRookA);
    blackPieces.push_back(bRookH);

    auto bKnightB = std::make_shared<Knight>(Color::Black);
    auto bKnightG = std::make_shared<Knight>(Color::Black);
    board.setPiece(0, 1, bKnightB);
    board.setPiece(0, 6, bKnightG);
    blackPieces.push_back(bKnightB);
    blackPieces.push_back(bKnightG);

    auto bBishopC = std::make_shared<Bishop>(Color::Black);
    auto bBishopF = std::make_shared<Bishop>(Color::Black);
    board.setPiece(0, 2, bBishopC);
    board.setPiece(0, 5, bBishopF);
    blackPieces.push_back(bBishopC);
    blackPieces.push_back(bBishopF);

    for (int col = 0; col < 8; ++col) {
        auto pawn = std::make_shared<Pawn>(Color::Black);
        board.setPiece(1, col, pawn);
        blackPieces.push_back(pawn);
    }

    // --- Fehér bábuk alul ---
    auto wKing = std::make_shared<King>(Color::White);
    board.setPiece(7, 4, wKing);
    whitePieces.push_back(wKing); // király mindig első

    auto wQueen = std::make_shared<Queen>(Color::White);
    board.setPiece(7, 3, wQueen);
    whitePieces.push_back(wQueen);

    auto wRookA = std::make_shared<Rook>(Color::White);
    auto wRookH = std::make_shared<Rook>(Color::White);
    board.setPiece(7, 0, wRookA);
    board.setPiece(7, 7, wRookH);
    whitePieces.push_back(wRookA);
    whitePieces.push_back(wRookH);

    auto wKnightB = std::make_shared<Knight>(Color::White);
    auto wKnightG = std::make_shared<Knight>(Color::White);
    board.setPiece(7, 1, wKnightB);
    board.setPiece(7, 6, wKnightG);
    whitePieces.push_back(wKnightB);
    whitePieces.push_back(wKnightG);

    auto wBishopC = std::make_shared<Bishop>(Color::White);
    auto wBishopF = std::make_shared<Bishop>(Color::White);
    board.setPiece(7, 2, wBishopC);
    board.setPiece(7, 5, wBishopF);
    whitePieces.push_back(wBishopC);
    whitePieces.push_back(wBishopF);

    for (int col = 0; col < 8; ++col) {
        auto pawn = std::make_shared<Pawn>(Color::White);
        board.setPiece(6, col, pawn);
        whitePieces.push_back(pawn);
    }

    // Fehér kezd
    currentTurn = Color::White;
}


bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;
    if (piece->getColor() != currentTurn) return false;

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

    // 3. Ha van fogás, töröljük a captured bábut a vektorból
    auto captured = board.getPiece(toRow, toCol);
    if (captured && !chosenMove.isEnPassant) {
        auto& vec = (captured->getColor() == Color::White) ? whitePieces : blackPieces;
        vec.erase(std::remove(vec.begin(), vec.end(), captured), vec.end());
    }

    // 4. Végrehajtás
    if (chosenMove.isCastle) {
        int dir = (toCol > fromCol) ? 1 : -1;
        int rookCol = (dir == 1) ? 7 : 0;
        int rookTargetCol = fromCol + dir;
        board.movePiece(fromRow, fromCol, toRow, toCol);             // király
        board.movePiece(fromRow, rookCol, fromRow, rookTargetCol);   // bástya
        auto king = std::dynamic_pointer_cast<King>(board.getPiece(toRow, toCol));
        if (king) king->setMoved(true);
        auto rook = std::dynamic_pointer_cast<Rook>(board.getPiece(fromRow, rookTargetCol));
        if (rook) rook->setMoved(true);
    }
    else if (chosenMove.isEnPassant) {
        board.movePiece(fromRow, fromCol, toRow, toCol);
        int capturedRow = (piece->getColor() == Color::White) ? toRow + 1 : toRow - 1;
        auto epCaptured = board.getPiece(capturedRow, toCol);
        if (epCaptured) {
            auto& vec = (epCaptured->getColor() == Color::White) ? whitePieces : blackPieces;
            vec.erase(std::remove(vec.begin(), vec.end(), epCaptured), vec.end());
        }
        board.setPiece(capturedRow, toCol, nullptr);
    }
    else {
        board.movePiece(fromRow, fromCol, toRow, toCol);
    }

    // 5. Promóció (fixen vezér)
    auto movedPiece = board.getPiece(toRow, toCol);
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        int promotionRow = (movedPiece->getColor() == Color::White) ? 0 : 7;
        if (toRow == promotionRow) {
            auto newQueen = std::make_shared<Queen>(movedPiece->getColor());
            board.setPiece(toRow, toCol, newQueen);

            auto& vec = (newQueen->getColor() == Color::White) ? whitePieces : blackPieces;
            vec.erase(std::remove(vec.begin(), vec.end(), movedPiece), vec.end());
            vec.push_back(newQueen);

            movedPiece = newQueen;
        }
    }

    // 6. Flag-ek frissítése és király pozíció
    if (movedPiece) {
        if (movedPiece->getType() == PieceType::King) {
            static_cast<King*>(movedPiece.get())->setMoved(true);
            if (movedPiece->getColor() == Color::White)
                whiteKingPos = { toRow, toCol };
            else
                blackKingPos = { toRow, toCol };
        }
        else if (movedPiece->getType() == PieceType::Rook) {
            static_cast<Rook*>(movedPiece.get())->setMoved(true);
        }
    }

    enPassantTarget.reset();
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        if (std::abs(toRow - fromRow) == 2) {
            int midRow = (fromRow + toRow) / 2;
            enPassantTarget = { midRow, toCol };
        }
    }

    // 7. Következő játékos
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
    return true;
}



std::vector<Move> Game::getLegalMoves(int fromRow, int fromCol) const {
    std::vector<Move> legalMoves;
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != currentTurn) return legalMoves;

    // 1. pszeudo-lépések a bábutól
    auto pseudoMoves = piece->getPossibleMoves(board, fromRow, fromCol);

    // 2. szűrés sakk ellen
    for (const auto& move : pseudoMoves) {
        if (!wouldBeInCheckAfterMove(fromRow, fromCol, move.toRow, move.toCol)) {
            legalMoves.push_back(move);
        }
    }

    // speciális lépések csak ha kell
    switch (piece->getType()) {
    case PieceType::King:
        addCastlingMoves(fromRow, fromCol, legalMoves);
        break;
    case PieceType::Pawn:
        addEnPassantMoves(fromRow, fromCol, legalMoves);
        addPromotionMoves(legalMoves);
        break;
    default:
        break;
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
            if (wouldBeInCheckAfterMove(row, col, row, checkCol)) {
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


bool Game::wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const {
    Board tempBoard = board;
    tempBoard.movePiece(fromRow, fromCol, toRow, toCol);

    auto movingPiece = board.getPiece(fromRow, fromCol);
    if (!movingPiece) return false;
    Color movingColor = movingPiece->getColor();

    // kiinduló királypozíció a cache-ből
    int kingRow, kingCol;
    if (movingColor == Color::White) {
        kingRow = whiteKingPos.first;
        kingCol = whiteKingPos.second;
    }
    else {
        kingRow = blackKingPos.first;
        kingCol = blackKingPos.second;
    }

    // ha maga a király lépett, akkor a célmező lesz az új pozíció
    if (movingPiece->getType() == PieceType::King) {
        kingRow = toRow;
        kingCol = toCol;
    }

    // --- Pawn attacks ---
    int dir = (movingColor == Color::White) ? -1 : 1;
    for (int dc : {-1, 1}) {
        int r = kingRow + dir, c = kingCol + dc;
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() != movingColor && p->getType() == PieceType::Pawn) return true;
        }
    }

    // --- Knight attacks ---
    int knightOffsets[8][2] = {
        {-2,-1},{-2,1},{-1,-2},{-1,2},
        {1,-2},{1,2},{2,-1},{2,1}
    };
    for (auto& off : knightOffsets) {
        int r = kingRow + off[0], c = kingCol + off[1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() != movingColor && p->getType() == PieceType::Knight) {
                return true;
            }
        }
    }

    // --- Bishop / Queen diagonals ---
    int diagDirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    for (auto& d : diagDirs) {
        int r = kingRow + d[0], c = kingCol + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p) {
                if (p->getColor() != movingColor &&
                    (p->getType() == PieceType::Bishop || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // --- Rook / Queen lines ---
    int lineDirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& d : lineDirs) {
        int r = kingRow + d[0], c = kingCol + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = tempBoard.getPiece(r, c);
            if (p) {
                if (p->getColor() != movingColor &&
                    (p->getType() == PieceType::Rook || p->getType() == PieceType::Queen)) {
                    return true;
                }
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // --- King (szomszédos mezők) ---
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = kingRow + dr, c = kingCol + dc;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                auto p = tempBoard.getPiece(r, c);
                if (p && p->getColor() != movingColor && p->getType() == PieceType::King) {
                    return true;
                }
            }
        }
    }

    return false; // nincs sakk
}

Color Game::getCurrentTurn() const {
	return currentTurn;
}


const Board& Game::getBoard() const {
	return board;
}