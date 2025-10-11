#include <cassert>
#include <iostream>
#include "Game.h"

// K-fehér király, q-fekete vezér, r-fekete bástya

void runAttackedTest() {
    Game g;
    std::string pos =
        "k . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . q . . .\n"
        ". . . . . . . .\n"
        "r . . . K . . .\n";
    g.loadPosition(pos, Color::Black);
    bool isAttacked = g.isSquareAttacked(g.getBitboard(), 7, 4, Color::White); // az isCheckMate, isStalemate függvények használják, bemenetként a defender színt várja
    assert(isAttacked);
    std::cout << "testSquareAttacked passed\n";
}

void runCheckmateTest() {
    Game g;
    std::string pos =
        "k . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n" 
        ". . . . q . . .\n"
        ". . . . . . . .\n"
        "r . . . K . . .\n";
	g.loadPosition(pos, Color::White); //a lépés után ellenőrizzük az ellenfélét, mielőtt átállítjuk a kört, ezért Black
    bool isCheckmate = g.isCheckmate();
    assert(isCheckmate);
    std::cout << "test Checkmate passed\n";
}

void runStalemateTest() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "r . . . . . r .\n"
        ". . . . . . . K\n";
    g.loadPosition(pos, Color::White); //a lépés után ellenőrizzük az ellenfélét, mielőtt átállítjuk a kört, ezért Black
    bool isStalemate = g.isStalemate();
    assert(isStalemate);
    std::cout << "test Stalemate passed\n";
}

void runWouldBeInCheckTest() {
    Game g;
    std::string pos =
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "r . . . P . . K\n";
    g.loadPosition(pos, Color::White); //a fehér paraszt nem tud lépni, mert utána sakkban állnánk
    auto moves = g.getLegalMoves(7, 4);
    assert(moves.empty());
    std::cout << "test wouldBeInCheck passed\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "No test group specified!\n";
        return 1;
    }
    std::string which = argv[1];
    if (which == "checkmate")  runCheckmateTest();
    else if (which == "attacked") runAttackedTest();
    else if (which == "stalemate") runStalemateTest();
    else if (which == "wouldbeincheck") runWouldBeInCheckTest();
    else {
        std::cerr << "Unknown test group: " << which << "\n";
        return 1;
    }
    return 0;
}
