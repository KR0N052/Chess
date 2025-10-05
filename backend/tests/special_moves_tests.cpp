#include <cassert>
#include <iostream>
#include "Game.h"

//castling tests
void test_castling_available() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "R . . . K . . R\n";
    // fehér király (7,4), bástyák (7,0) és (7,7), köztük üres
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 4); // király lépései
    
    bool canCastleKingside = false;
    bool canCastleQueenside = false;

    for (auto& m : moves) {
        if (m.toRow == 7 && m.toCol == 6) canCastleKingside = true;
        if (m.toRow == 7 && m.toCol == 2) canCastleQueenside = true;
    }
    
    assert(canCastleKingside);
    assert(canCastleQueenside);

    std::cout << "test_kingside_castling_available passed\n";
}
void test_castling_blocked_by_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "R N . . K . . R\n";
    // van egy knight a (7,1)-en, blokkolja a vezéroldali sáncot
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 4);
    bool canCastleQueenside = false;
    for (auto& m : moves) {
        if (m.toRow == 7 && m.toCol == 2) canCastleQueenside = true;
    }
    assert(!canCastleQueenside);
    std::cout << "test_castling_blocked_by_piece passed\n";
}
void test_castling_blocked_by_check() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
		". . . r . . . .\n" // fekete bástya (4,2) a király átlépne sakkon a vezéroldali sáncolásnál
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "R . . . K . . R\n";
    g.loadPosition(pos, Color::White);
    auto moves = g.getLegalMoves(7, 4);
    bool canCastleKingside = false;
    bool canCastleQueenside = false;
    for (auto& m : moves) {
        if (m.toRow == 7 && m.toCol == 6) canCastleKingside = true;
        if (m.toRow == 7 && m.toCol == 2) canCastleQueenside = true;
    }
    assert(canCastleKingside);
    assert(!canCastleQueenside);
    std::cout << "test_castling_blocked_by_check passed\n";
}
void runCastlingTests() {
    test_castling_available();
    test_castling_blocked_by_piece();
    test_castling_blocked_by_check();
    std::cout << "All castling tests passed!\n";
}

//promotion tests
void test_white_pawn_promotion_to_queen() {
    Game g;
    std::string pos =
        " . . . . . . . .\n"
        " P . . . . . . .\n"
        " . . . . . . . .\n"
        " . . . . . . . .\n"
        " . . . . . . . .\n"
        " . . . . . . . .\n"
        " . . . . . . . .\n"
        " . . . . . . . .\n";
    // fehér pawn a (1,0)-n, egy lépésre a promotiontól
    g.loadPosition(pos, Color::White);

	g.makeMove(1, 0, 0, 0); // lép a túloldalra
	PieceType promotedType = g.getBoard().getPiece(0, 0)->getType();
    assert(promotedType == PieceType::Queen);
    std::cout << "test_white_pawn_promotion_to_queen passed\n";
}
void runPromotionTests() {
    test_white_pawn_promotion_to_queen();
    std::cout << "All promotion tests passed!\n";
}

//en passant tests
void test_en_passant() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . p . . . . .\n"
        ". . . . . . . .\n"
        ". . . P . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n";
    // fekete pawn (3,2), fehér pawn (3,3)
    g.loadPosition(pos, Color::Black);
    g.makeMove(1, 2, 3, 2); // fekete pawn lép kétet előre
    /*  jelenlegi allas:
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . p P . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
    */
    auto moves = g.getLegalMoves(3, 3); // fehér pawn lépései
    bool canEnPassant = false;
    for (auto& m : moves) {
        if (m.toRow == 2 && m.toCol == 2 && m.isEnPassant) canEnPassant = true;
    }
    assert(canEnPassant);
    g.makeMove(3, 3, 2, 2); // fehér pawn en passant lép
    assert(g.getBoard().getPiece(3, 2) == nullptr); // a fekete pawn eltűnt
    std::cout << "test_en_passant passed\n";
}
void runEnPassantTests() {
	test_en_passant();
    std::cout << "All en passant tests passed!\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "No test group specified!\n";
        return 1;
    }
    std::string which = argv[1];
    if (which == "castle")   runCastlingTests();
    else if (which == "promotion") runPromotionTests();
    else if (which == "enpassant") runEnPassantTests();
    else {
        std::cerr << "Unknown test group: " << which << "\n";
        return 1;
    }
    return 0;
}