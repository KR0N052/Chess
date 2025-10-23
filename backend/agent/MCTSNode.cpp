#include "MCTSNode.h"
#include <cmath>


MCTSNode* MCTSNode::selectChild() {
	
	int maxValue = -1;
	MCTSNode* maxChild;
	for (auto child : children) {
		if (child->uctValue  > maxValue) {
			maxValue = child->visits;
			maxChild = child;
		}
	}
	if (maxValue != -1) {
		return maxChild;
	}
	return nullptr;	
}

void MCTSNode::updateUCTValue() {
	if (visits == 0) {
		// Ha még nem látogattuk, végtelennek tekintjük, hogy biztosan kipróbáljuk
		uctValue = std::numeric_limits<double>::infinity();
		return;
	}

	int parentVisits = (parent != nullptr) ? parent->visits : 1;
	double exploitation = wins / visits;
	double exploration = std::sqrt(2) * std::sqrt(std::log(parentVisits) / visits);

	uctValue = exploitation + exploration;
}