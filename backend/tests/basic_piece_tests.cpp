#include <cassert>
#include <iostream>
#include "Game.h"

//Pawn tests
void test_white_pawn_initial_double_step() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". P . . . . . .\n"
        ". . . . . . . .\n";
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(6, 1); // fehér pawn a (6,1)-en
    assert(moves.size() == 2);          // előre 1 és előre 2
    std::cout << "test_white_pawn_initial_double_step passed\n";
}
void test_white_pawn_blocked() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". p . . . . . .\n"
        ". P . . . . . .\n"
        ". . . . . . . .\n";
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(6, 1);
    assert(moves.empty()); // előtte áll egy fekete pawn
    std::cout << "test_white_pawn_blocked passed\n";
}
void test_white_pawn_capture() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p . . . . . . .\n"
        ". P . . . . . .\n"
        ". . . . . . . .\n";
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(6, 1);
    bool hasCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 5 && m.toCol == 0) hasCapture = true;
    }
    assert(hasCapture);
    std::cout << "test_white_pawn_capture passed\n";
}
void runPawnTests() {
    test_white_pawn_initial_double_step();
    test_white_pawn_blocked();
    test_white_pawn_capture();
    std::cout << "All pawn tests passed!\n";
}

//Rook tests
void test_rook_open_file() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "R . . . . . . .\n"; // fehér rook a (7,0)-n
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 0);
    // 7. sorban jobbra 7 lépés, felfelé 7 lépés = 14
    assert(moves.size() == 14);
    std::cout << "test_rook_open_file passed\n";
}
void test_rook_blocked_by_own_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "P . . . . . . .\n"
        "R . . . . . . .\n"; // rook (7,0), előtte saját pawn (6,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 0);
    // csak jobbra tud menni (7 lépés), felfelé nem
    assert(moves.size() == 7);
    std::cout << "test_rook_blocked_by_own_piece passed\n";
}
void test_rook_capture_enemy() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p . . . . . . .\n"
        ". . . . . . . .\n"
        "R . . . . . . .\n"; // rook (7,0), ellenséges pawn (5,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 0);
    bool canCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 5 && m.toCol == 0) canCapture = true;
    }
    assert(canCapture);
    std::cout << "test_rook_capture_enemy passed\n";
}
void runRookTests() {
    test_rook_open_file();
    test_rook_blocked_by_own_piece();
    test_rook_capture_enemy();
    std::cout << "All rook tests passed!\n";
}

//Knight tests
void test_knight_open_moves() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . N . . . . .\n"; // fehér knight (7,2)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    assert(moves.size() == 4);
    std::cout << "test_knight_open_moves passed\n";
}
void test_knight_jump_over_pieces() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p p p p p p p p\n"
        ". . N . . . . .\n"; // knight (7,2), előtte tele a sor
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    assert(moves.size() == 4);
    std::cout << "test_knight_jump_over_pieces passed\n";
}
void test_knight_capture_enemy() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p . . . . . . .\n"
        ". . N . . . . .\n"; // knight (7,2), ellenséges pawn (6,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool canCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 0) canCapture = true;
    }
    assert(canCapture);
    std::cout << "test_knight_capture_enemy passed\n";
}
void test_knight_blocked_by_own_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "P . . . . . . .\n"
        ". . N . . . . .\n"; // knight (7,2), saját pawn (6,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool hitsOwn = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 0) hitsOwn = true;
    }
    assert(!hitsOwn); // nem léphet saját bábu helyére
    std::cout << "test_knight_blocked_by_own_piece passed\n";
}
void runKnightTests() {
    test_knight_open_moves();
    test_knight_jump_over_pieces();
    test_knight_capture_enemy();
    test_knight_blocked_by_own_piece();
    std::cout << "All knight tests passed!\n";
}

//Bishop tests
void test_bishop_open_diagonals() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . B . . . . .\n"; // fehér bishop (7,2)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    assert(moves.size() == 2 + 5);
    std::cout << "test_bishop_open_diagonals passed\n";
}
void test_bishop_blocked_by_own_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". P . . . . . .\n"
        ". . B . . . . .\n"; // bishop (7,2), saját pawn (6,1)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool hitsOwn = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 1) hitsOwn = true;
    }
    assert(!hitsOwn); // nem léphet saját bábu helyére
    std::cout << "test_bishop_blocked_by_own_piece passed\n";
}
void test_bishop_capture_enemy() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p . . . . . . .\n"
        ". . . . . . . .\n"
        ". . B . . . . .\n"; // bishop (7,2), ellenséges pawn (5,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool canCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 5 && m.toCol == 0) canCapture = true;
    }
    assert(canCapture);
    std::cout << "test_bishop_capture_enemy passed\n";
}
void runBishopTests() {
    test_bishop_open_diagonals();
    test_bishop_blocked_by_own_piece();
    test_bishop_capture_enemy();
    std::cout << "All bishop tests passed!\n";
}

//Queen tests
void test_queen_open_board() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . Q . . . . .\n"; // fehér queen (7,2)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    
    assert(moves.size() == 21);
    std::cout << "test_queen_open_board passed\n";
}
void test_queen_blocked_by_own_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". P . . . . . .\n"
        ". . Q . . . . .\n"; // queen (7,2), saját pawn (6,1)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool hitsOwn = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 1) hitsOwn = true;
    }
    assert(!hitsOwn); // nem léphet saját bábu helyére
    std::cout << "test_queen_blocked_by_own_piece passed\n";
}
void test_queen_capture_enemy() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "p . . . . . . .\n"
        ". . . . . . . .\n"
        ". . Q . . . . .\n"; // queen (7,2), ellenséges pawn (5,0)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool canCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 5 && m.toCol == 0) canCapture = true;
    }
    assert(canCapture);
    std::cout << "test_queen_capture_enemy passed\n";
}
void runQueenTests() {
    test_queen_open_board();
    test_queen_blocked_by_own_piece();
    test_queen_capture_enemy();
    std::cout << "All queen tests passed!\n";
}

//King tests
void test_king_open_moves() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . K . . . . .\n"; // fehér király (7,2)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    // A (7,2)-ről 5 legális lépés van (felfelé 3, oldalra 2)
    assert(moves.size() == 5);
    std::cout << "test_king_open_moves passed\n";
}
void test_king_blocked_by_own_piece() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". P . . . . . .\n"
        ". . K . . . . .\n"; // király (7,2), saját pawn (6,1)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool hitsOwn = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 1) hitsOwn = true;
    }
    assert(!hitsOwn); // nem léphet saját bábu helyére
    std::cout << "test_king_blocked_by_own_piece passed\n";
}
void test_king_capture_enemy() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". p . . . . . .\n"
        ". . K . . . . .\n"; // király (7,2), ellenséges pawn (6,1)
    g.loadPosition(pos, Color::White);

    auto moves = g.getLegalMoves(7, 2);
    bool canCapture = false;
    for (auto& m : moves) {
        if (m.toRow == 6 && m.toCol == 1) canCapture = true;
    }
    assert(canCapture);
    std::cout << "test_king_capture_enemy passed\n";
}
void runKingTests() {
    test_king_open_moves();
    test_king_blocked_by_own_piece();
    test_king_capture_enemy();
    std::cout << "All king tests passed!\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "No test group specified!\n";
        return 1;
    }
    std::string which = argv[1];
    switch (which[0]) {
    case 'p': runPawnTests();   break;
    case 'r': runRookTests();   break;
    case 'n': runKnightTests(); break;
    case 'b': runBishopTests(); break;
    case 'q': runQueenTests();  break;
    case 'k': runKingTests();   break;
    default:
        std::cerr << "Unknown test group: " << which << "\n";
        return 1;
    }
    return 0;
}



