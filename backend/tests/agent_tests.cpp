#include <iostream>
#include <cassert>
#include "../backend/agent/MCTSAgent.h"


void runSimulateTest() {
    // Példa teszt
	MCTSAgent agent;
	Game g = *agent.getGame();
    for (int i = 0; i < 1000; i++) {
        g.reset();
        bool result = agent.simulateRandomGame(g);
    }
        
    std::cout << "MCTS simulation tests completed.\n";
    
    assert(true);
    
}




int main(int argc, char** argv) {
    if (argc < 1) {
        std::cerr << "No test group specified!\n";
        return 1;
    }
    std::string which = argv[1];
    if (which == "mcts") {
        runSimulateTest();
    }
    else {
        std::cerr << "Unknown test group: " << which << "\n";
    }
    
    return 0;
}