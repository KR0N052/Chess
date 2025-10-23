#include "Game.h"
#include "MCTSNode.h"

class MCTSAgent {
private:
	MCTSNode* root;
	Game game;	// aktuális játékállás
	Game tempGame;	// ideiglenes játékállás a szimulációkhoz

public:
	Game* getGame();
	Move chooseMove();

	MCTSNode* selection(); //returns the first not fully expanded node
	MCTSNode* expansion(MCTSNode* node);	// returns a newly created child node
	bool simulateRandomGame(Game game);		// returns true if white wins, false if black wins
	void backpropagation(MCTSNode* node, bool win);		// backpropagates the result up to the root
	
	void computeMCTSTree(int iterations = 1000);

	MCTSAgent() {
		root = new MCTSNode(nullptr, Move(-1, -1, -1, -1), Color::White);
	}
};