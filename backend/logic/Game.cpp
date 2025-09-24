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


// ---------------- Implementáció ----------------
Game::Game() {
    resetBoard();
}


void Game::resetBoard() {
    board.clear();
    whitePieceSquares.clear();
    blackPieceSquares.clear();

    // --- Fekete bábuk felül ---
    board.setPiece(0, 4, std::make_shared<King>(Color::Black));
    blackPieceSquares.push_back({ 0, 4 }); // király mindig első

    board.setPiece(0, 3, std::make_shared<Queen>(Color::Black));
    blackPieceSquares.push_back({ 0, 3 });

    board.setPiece(0, 0, std::make_shared<Rook>(Color::Black));
    board.setPiece(0, 7, std::make_shared<Rook>(Color::Black));
    blackPieceSquares.push_back({ 0, 0 });
    blackPieceSquares.push_back({ 0, 7 });

    board.setPiece(0, 1, std::make_shared<Knight>(Color::Black));
    board.setPiece(0, 6, std::make_shared<Knight>(Color::Black));
    blackPieceSquares.push_back({ 0, 1 });
    blackPieceSquares.push_back({ 0, 6 });

    board.setPiece(0, 2, std::make_shared<Bishop>(Color::Black));
    board.setPiece(0, 5, std::make_shared<Bishop>(Color::Black));
    blackPieceSquares.push_back({ 0, 2 });
    blackPieceSquares.push_back({ 0, 5 });

    for (int col = 0; col < 8; ++col) {
        board.setPiece(1, col, std::make_shared<Pawn>(Color::Black));
        blackPieceSquares.push_back({ 1, col });
    }

    // --- Fehér bábuk alul ---
    board.setPiece(7, 4, std::make_shared<King>(Color::White));
    whitePieceSquares.push_back({ 7, 4 }); // király mindig első

    board.setPiece(7, 3, std::make_shared<Queen>(Color::White));
    whitePieceSquares.push_back({ 7, 3 });

    board.setPiece(7, 0, std::make_shared<Rook>(Color::White));
    board.setPiece(7, 7, std::make_shared<Rook>(Color::White));
    whitePieceSquares.push_back({ 7, 0 });
    whitePieceSquares.push_back({ 7, 7 });

    board.setPiece(7, 1, std::make_shared<Knight>(Color::White));
    board.setPiece(7, 6, std::make_shared<Knight>(Color::White));
    whitePieceSquares.push_back({ 7, 1 });
    whitePieceSquares.push_back({ 7, 6 });

    board.setPiece(7, 2, std::make_shared<Bishop>(Color::White));
    board.setPiece(7, 5, std::make_shared<Bishop>(Color::White));
    whitePieceSquares.push_back({ 7, 2 });
    whitePieceSquares.push_back({ 7, 5 });

    for (int col = 0; col < 8; ++col) {
        board.setPiece(6, col, std::make_shared<Pawn>(Color::White));
        whitePieceSquares.push_back({ 6, col });
    }

    // Fehér kezd
    currentTurn = Color::White;

    // Királypozíciók inicializálása
    whiteKingPos = { 7, 4 };
    blackKingPos = { 0, 4 };

    // En passant reset
    enPassantTarget.reset();
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

    // 3. Ha van fogás, töröljük a captured koordinátát a listából
    auto captured = board.getPiece(toRow, toCol);
    if (captured && !chosenMove.isEnPassant) {
        auto& vec = (captured->getColor() == Color::White) ? whitePieceSquares : blackPieceSquares;
        vec.erase(std::remove(vec.begin(), vec.end(), std::make_pair(toRow, toCol)), vec.end());
    }

    // 4. Végrehajtás
    if (chosenMove.isCastle) {
        int dir = (toCol > fromCol) ? 1 : -1;
        int rookCol = (dir == 1) ? 7 : 0;
        int rookTargetCol = fromCol + dir;

        board.movePiece(fromRow, fromCol, toRow, toCol);             // király
        board.movePiece(fromRow, rookCol, fromRow, rookTargetCol);   // bástya

        // koordinátalisták frissítése
        auto& kingVec = (piece->getColor() == Color::White) ? whitePieceSquares : blackPieceSquares;
        auto& rookVec = kingVec;
        // király koordinátájának átírása
        auto it = std::find(kingVec.begin(), kingVec.end(), std::make_pair(fromRow, fromCol));
        if (it != kingVec.end()) *it = { toRow, toCol };
        // bástya koordinátájának átírása
        auto itR = std::find(rookVec.begin(), rookVec.end(), std::make_pair(fromRow, rookCol));
        if (itR != rookVec.end()) *itR = { fromRow, rookTargetCol };

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
            auto& vec = (epCaptured->getColor() == Color::White) ? whitePieceSquares : blackPieceSquares;
            vec.erase(std::remove(vec.begin(), vec.end(), std::make_pair(capturedRow, toCol)), vec.end());
        }
        board.setPiece(capturedRow, toCol, nullptr);

        // lépő bábu koordinátájának frissítése
        auto& moverVec = (piece->getColor() == Color::White) ? whitePieceSquares : blackPieceSquares;
        auto it = std::find(moverVec.begin(), moverVec.end(), std::make_pair(fromRow, fromCol));
        if (it != moverVec.end()) *it = { toRow, toCol };
    }
    else {
        board.movePiece(fromRow, fromCol, toRow, toCol);

        // lépő bábu koordinátájának frissítése
        auto& moverVec = (piece->getColor() == Color::White) ? whitePieceSquares : blackPieceSquares;
        auto it = std::find(moverVec.begin(), moverVec.end(), std::make_pair(fromRow, fromCol));
        if (it != moverVec.end()) *it = { toRow, toCol };
    }

    // 5. Promóció (fixen vezér)
    auto movedPiece = board.getPiece(toRow, toCol);
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        int promotionRow = (movedPiece->getColor() == Color::White) ? 0 : 7;
        if (toRow == promotionRow) {
            auto newQueen = std::make_shared<Queen>(movedPiece->getColor());
            board.setPiece(toRow, toCol, newQueen);
            movedPiece = newQueen;
            // koordinátalistában nem kell változtatni, mert a mező ugyanaz
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

    // 7. En passant target
    enPassantTarget.reset();
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        if (std::abs(toRow - fromRow) == 2) {
            int midRow = (fromRow + toRow) / 2;
            enPassantTarget = { midRow, toCol };
        }
    }

    // 8. Következő játékos
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;

    //if (isCheckmate()) {
    //    std::cout << "Checkmate! "
    //        << ((currentTurn == Color::White) ? "Black" : "White")
    //        << " wins.\n";
    //}
    //else if (isStalemate()) {
    //    std::cout << "Stalemate! Draw.\n";
    //}

    return true;
}




std::vector<Move> Game::getLegalMoves(int fromRow, int fromCol) const {
    std::vector<Move> legalMoves;
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != currentTurn) return legalMoves;

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
        if (move.isEnPassant || move.isPromotion()) {
            if (!safeWouldBeInCheckAfterMove(fromRow, fromCol, move.toRow, move.toCol)) {
				legalMoves.push_back(move);
            }
        }
        else if (move.isCastle) {
			legalMoves.push_back(move); // sáncolásnál már ellenőriztük a sakkot (neki spéci szabály van)
        }
        else{
            if (!fastWouldBeInCheckAfterMove(fromRow, fromCol, move.toRow, move.toCol)) {
                legalMoves.push_back(move);
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
            if (safeWouldBeInCheckAfterMove(row, col, row, checkCol)) {
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

bool Game::fastWouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;
    Color movingColor = piece->getColor();

    auto captured = board.getPiece(toRow, toCol);

    // ideiglenesen áthelyezzük a bábut
    board.setPiece(toRow, toCol, piece);
    board.setPiece(fromRow, fromCol, nullptr);

    // lokális kingPos számítás
    std::pair<int, int> kingPos = (movingColor == Color::White) ? whiteKingPos : blackKingPos;
    if (piece->getType() == PieceType::King) {
        kingPos = { toRow, toCol };
    }

    bool inCheck = isSquareAttacked(board, kingPos.first, kingPos.second, movingColor);

    // visszaállítjuk a táblát
    board.setPiece(fromRow, fromCol, piece);
    board.setPiece(toRow, toCol, captured);

    return inCheck;
}

bool Game::safeWouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const {
    auto movingPiece = board.getPiece(fromRow, fromCol);
    if (!movingPiece) return false;
    Color movingColor = movingPiece->getColor();

    Board tempBoard = board;
    tempBoard.movePiece(fromRow, fromCol, toRow, toCol);

    int kingRow = (movingPiece->getType() == PieceType::King)
        ? toRow
        : (movingColor == Color::White ? whiteKingPos.first : blackKingPos.first);
    int kingCol = (movingPiece->getType() == PieceType::King)
        ? toCol
        : (movingColor == Color::White ? whiteKingPos.second : blackKingPos.second);

    return isSquareAttacked(tempBoard, kingRow, kingCol, movingColor);
}

bool Game::isSquareAttacked(const Board& b, int row, int col, Color defender) const {
    // Pawn
    int dir = (defender == Color::White) ? -1 : 1;
    for (int dc : {-1, 1}) {
        int r = row + dir, c = col + dc;
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = b.getPiece(r, c);
            if (p && p->getColor() != defender && p->getType() == PieceType::Pawn) return true;
        }
    }

    // Knight
    int knightOffsets[8][2] = { {-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1} };
    for (auto& off : knightOffsets) {
        int r = row + off[0], c = col + off[1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = b.getPiece(r, c);
            if (p && p->getColor() != defender && p->getType() == PieceType::Knight) return true;
        }
    }

    // Bishop / Queen diagonals
    int diagDirs[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    for (auto& d : diagDirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = b.getPiece(r, c);
            if (p) {
                if (p->getColor() != defender &&
                    (p->getType() == PieceType::Bishop || p->getType() == PieceType::Queen)) return true;
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // Rook / Queen lines
    int lineDirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& d : lineDirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto p = b.getPiece(r, c);
            if (p) {
                if (p->getColor() != defender &&
                    (p->getType() == PieceType::Rook || p->getType() == PieceType::Queen)) return true;
                break;
            }
            r += d[0]; c += d[1];
        }
    }

    // King
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr, c = col + dc;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                auto p = b.getPiece(r, c);
                if (p && p->getColor() != defender && p->getType() == PieceType::King) return true;
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
    // 1. Aktuális király pozíciója
    std::pair<int, int> kingPos = (currentTurn == Color::White) ? whiteKingPos : blackKingPos;

    // 2. Sakkban van‑e?
    bool inCheck = isSquareAttacked(board, kingPos.first, kingPos.second, currentTurn);
    if (!inCheck) return false;

    // 3. Van‑e legális lépés?
    const auto& coords = (currentTurn == Color::White) ? whitePieceSquares : blackPieceSquares;
    for (auto [r, c] : coords) {
        auto piece = board.getPiece(r, c);
        if (!piece) continue;
        auto moves = getLegalMoves(r, c);
        if (!moves.empty()) return false;
    }

    return true; // sakkban van és nincs lépés → matt
}

bool Game::isStalemate() const {
    // 1. Aktuális király pozíciója
    std::pair<int, int> kingPos = (currentTurn == Color::White) ? whiteKingPos : blackKingPos;

    // 2. Ha sakkban van, nem lehet pat
    bool inCheck = isSquareAttacked(board, kingPos.first, kingPos.second, currentTurn);
    if (inCheck) return false;

    // 3. Van‑e legális lépés?
    const auto& coords = (currentTurn == Color::White) ? whitePieceSquares : blackPieceSquares;
    for (auto [r, c] : coords) {
        auto piece = board.getPiece(r, c);
        if (!piece) continue;
        auto moves = getLegalMoves(r, c);
        if (!moves.empty()) return false;
    }

    return true; // nincs sakk és nincs lépés → pat
}

