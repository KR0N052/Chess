#include "Game.h"
#include "pieces/Piece.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "pieces/Pawn.h"
#include <memory> // std::shared_ptr, std::make_shared
#include <cmath> // std::abs

Game::Game() {
    resetBoard();
}

struct LastMove {
    int fromRow, fromCol, toRow, toCol;
    std::shared_ptr<Piece> piece;
};
LastMove lastMove;

void Game::resetBoard() {
    board.clear();

    // --- Fehér bábuk ---
    board.setPiece(0, 0, std::make_shared<Rook>(Color::White));
    board.setPiece(0, 1, std::make_shared<Knight>(Color::White));
    board.setPiece(0, 2, std::make_shared<Bishop>(Color::White));
    board.setPiece(0, 3, std::make_shared<Queen>(Color::White));
    board.setPiece(0, 4, std::make_shared<King>(Color::White));
    board.setPiece(0, 5, std::make_shared<Bishop>(Color::White));
    board.setPiece(0, 6, std::make_shared<Knight>(Color::White));
    board.setPiece(0, 7, std::make_shared<Rook>(Color::White));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(1, col, std::make_shared<Pawn>(Color::White));
    }

    // --- Fekete bábuk ---
    board.setPiece(7, 0, std::make_shared<Rook>(Color::Black));
    board.setPiece(7, 1, std::make_shared<Knight>(Color::Black));
    board.setPiece(7, 2, std::make_shared<Bishop>(Color::Black));
    board.setPiece(7, 3, std::make_shared<Queen>(Color::Black));
    board.setPiece(7, 4, std::make_shared<King>(Color::Black));
    board.setPiece(7, 5, std::make_shared<Bishop>(Color::Black));
    board.setPiece(7, 6, std::make_shared<Knight>(Color::Black));
    board.setPiece(7, 7, std::make_shared<Rook>(Color::Black));

    for (int col = 0; col < 8; ++col) {
        board.setPiece(6, col, std::make_shared<Pawn>(Color::Black));
    }

    // Fehér kezd
    currentTurn = Color::White;
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false; // nincs bábu ezen a mezőn

    // 1. Ellenőrzés: csak a soron következő játékos léphet
    if (piece->getColor() != currentTurn) {
        return false;
    }

    // 2. Megnézzük, hogy a lépés benne van-e a bábu lehetséges lépéseiben
    auto moves = piece->getPossibleMoves(board, fromRow, fromCol);

    std::vector<Move> legalMoves;
    for (const auto& move : moves) {
        if (!wouldBeInCheckAfterMove(fromRow, fromCol, move.toRow, move.toCol)) {
            legalMoves.push_back(move);
        }
    }
    
    bool validMove = false;
    for (const auto& move : legalMoves) {
        if (move.toRow == toRow && move.toCol == toCol) {
            validMove = true;
            break;
        }
    }

	// speciális lépések kezelése 

	// --- EN PASSANT ELLENŐRZÉS ---
    if (piece->getType() == PieceType::Pawn) {
        int dir = (piece->getColor() == Color::White) ? 1 : -1;
        if (toCol != fromCol && !board.getPiece(toRow, toCol)) {
            // átlós lépés üres mezőre → csak en passant lehet
            if (lastMove.piece && piece->getType() == PieceType::Pawn) {
                if (lastMove.toRow == fromRow && lastMove.toCol == toCol &&
                    abs(lastMove.fromRow - lastMove.toRow) == 2) {
                    // töröljük az ellenfél gyalogját
                    board.setPiece(lastMove.toRow, lastMove.toCol, nullptr);
                    validMove = true;
                }
            }
        }
    }

    // --- SÁNCOLÁS ELLENŐRZÉS ---
    if (piece->getType() == PieceType::King) {
		auto king = static_cast<King*>(piece.get());
        // Király sáncolni próbál (két oszlopot lép jobbra vagy balra)
        if (fromRow == toRow && abs(toCol - fromCol) == 2) {
            int dir = (toCol > fromCol) ? 1 : -1; // jobbra vagy balra
            int rookCol = (dir == 1) ? 7 : 0;     // bástya helye
            int rookTargetCol = fromCol + dir;   // bástya új helye

            auto rook = board.getPiece(fromRow, rookCol);
            if (rook && rook->getType() == PieceType::Rook) {
                // 1. Király és bástya nem lépett még
                if (!king->hasMoved() && !static_cast<Rook*>(rook.get())->hasMoved()) {
                    // 2. Köztes mezők üresek
                    bool pathClear = true;
                    for (int c = fromCol + dir; c != rookCol; c += dir) {
                        if (board.getPiece(fromRow, c)) {
                            pathClear = false;
                            break;
                        }
                    }
                    // 3. Király nem áll sakkban, és nem lép át sakkon
                    bool safe = true;
                    int kingCol = fromCol;
                    for (int step = 0; step <= 2; step++) {
                        int checkCol = fromCol + step * dir;
                        if (wouldBeInCheckAfterMove(fromRow, fromCol, fromRow, checkCol)) {
                            safe = false;
                            break;
                        }
                    }

                    if (pathClear && safe) {
                        // --- Sáncolás végrehajtása ---
                        board.movePiece(fromRow, fromCol, toRow, toCol); // király
                        board.movePiece(fromRow, rookCol, fromRow, rookTargetCol); // bástya

                        king->setMoved(true);
                        static_cast<Rook*>(rook.get())->setMoved(true);

                        currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
                        return true;
                    }
                }
            }
        } 
    }



    if (!validMove) return false;



    // 3. Lépés végrehajtása
    lastMove = { fromRow, fromCol, toRow, toCol, piece };
    board.movePiece(fromRow, fromCol, toRow, toCol);
	
    // --- Pawn promotion ellenőrzés ---
    auto movedPiece = board.getPiece(toRow, toCol);   // <-- friss bábu a célmezőről
    if (movedPiece && movedPiece->getType() == PieceType::Pawn) {
        int promotionRow = (movedPiece->getColor() == Color::White) ? 7 : 0;
        if (toRow == promotionRow) {
            board.setPiece(toRow, toCol, std::make_shared<Queen>(movedPiece->getColor()));
        }
    }

    // 4. Játékosváltás
    currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;

    return true;
}

bool Game::wouldBeInCheckAfterMove(int fromRow, int fromCol, int toRow, int toCol) const {
    // 1. Másolat a tábláról
    Board tempBoard = board;

    // 2. Lépés végrehajtása a másolaton
    tempBoard.movePiece(fromRow, fromCol, toRow, toCol);

    // 3. Megkeressük a lépő játékos királyának pozícióját
    Color movingColor = board.getPiece(fromRow, fromCol)->getColor();
    int kingRow = -1, kingCol = -1;
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() == movingColor && p->getType() == PieceType::King) {
                kingRow = r;
                kingCol = c;
                break;
            }
        }
    }

    // 4. Megnézzük, hogy az ellenfél bármelyik bábuja üti-e a királyt
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            auto p = tempBoard.getPiece(r, c);
            if (p && p->getColor() != movingColor) {
                auto moves = p->getPossibleMoves(tempBoard, r, c);
                for (auto& m : moves) {
                    if (m.toRow == kingRow && m.toCol == kingCol) {
                        return true; // sakkban lenne
                    }
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
