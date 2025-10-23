#include <vector>
#include "Move.h"
#include "../logic/Game.h"


class MCTSNode {
private:
	Move move; // The move that led to this node
	Color player; // The player who made the move
	int visits; // Number of times this node has been visited
	double wins; // Number of wins from this node
	double uctValue; // UCT value for selection
	MCTSNode* parent; // Pointer to the parent node
	std::vector<MCTSNode*> children; // Child nodes
	bool isFullyExpanded; // Whether all possible moves have been expanded
	bool isTerminal; // Whether this node represents a terminal state

public:
	MCTSNode(MCTSNode* _parent, Move _move, Color _player) : visits(0), wins(0.0), uctValue(0.0), parent(_parent), isFullyExpanded(false), isTerminal(false), move(_move), player(_player) {}

	MCTSNode* selectChild();

	bool getIsFullyExpanded() const { return isFullyExpanded; }

	void newChild(MCTSNode* child) { children.push_back(child); }

	void setFullyExpanded() { isFullyExpanded = true; }
	std::vector<MCTSNode*> getChildren() const { return children; }
	Move getMove() const { return move; }
	Color getPlayer() const { return player; }
	MCTSNode* getParent() const { return parent; }	
	void addVisit() {visits++;}
	void addWin(double value) { wins += value; }
	void updateUCTValue();
};