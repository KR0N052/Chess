#include "MoveValidator.h"

bool MoveValidator::isValidMove(const Board& board, const Move& move, Color currentPlayer) {
    const Piece& fromPiece = board.getPiece(move.fromRow, move.fromCol);
    const Piece& toPiece = board.getPiece(move.toRow, move.toCol);

    // Nem a saját bábunkkal próbálunk lépni
    if (fromPiece.color != currentPlayer) return false;

    // Nem léphetünk saját bábu helyére
    if (toPiece.color == currentPlayer) return false;

    // Példa: király léphet max 1 mezőt bármely irányba
    if (fromPiece.type == PieceType::King) {
        int dr = abs(move.toRow - move.fromRow);
        int dc = abs(move.toCol - move.fromCol);
        return (dr <= 1 && dc <= 1);
    }

    // Egyéb bábukhoz később jön a logika
    return false;
}
