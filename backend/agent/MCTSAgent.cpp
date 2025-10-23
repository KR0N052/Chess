#include "MCTSAgent.h"
#include "Logger.h"
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

Game* MCTSAgent::getGame() {
	return &game;
}	

Move MCTSAgent::chooseMove() {
	vector<Move> moves;
	vector<pair<int, int> > piecePositions;
	uint64_t bb = game.getBlackPieces();

	std::srand(std::time(nullptr));

	while (bb) {
		int sq = __builtin_ctzll(bb);	// legalsó beállított bit indexe
		bb &= (bb - 1);					// töröljük a feldolgozott bitet
		int r = sq / 8;
		int c = sq % 8;
		piecePositions.push_back({ r, c });
	}
	
	for(auto& pos : piecePositions) {
		auto pieceMoves = game.getLegalMoves(pos.first, pos.second);
		moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
	}
	if(moves.empty()) {
		Logger::debug("MCTSAgent::chooseMove: Nincs lépés!");
		return Move(-1, -1, -1, -1);
	}

	int chosenIndex = std::rand() % moves.size();

	return moves[chosenIndex];
}


MCTSNode* MCTSAgent::selection() {

	MCTSNode* firstNotFullyExpanded = root;
	while (firstNotFullyExpanded->getIsFullyExpanded()) {
		tempGame.applyMove(firstNotFullyExpanded->getMove());
		firstNotFullyExpanded = firstNotFullyExpanded->selectChild();
	}
	return firstNotFullyExpanded;
}

MCTSNode* MCTSAgent::expansion(MCTSNode* node) {
	vector<Move> possibleMoves;
	uint64_t bb = (tempGame.getCurrentTurn() == Color::White) ? tempGame.getWhitePieces() : tempGame.getBlackPieces();

	while (bb) {
		int sq = __builtin_ctzll(bb);	// legalsó beállított bit indexe
		bb &= (bb - 1);					// töröljük a feldolgozott bitet
		int r = sq / 8;
		int c = sq % 8;
		auto pieceMoves = tempGame.getLegalMoves(r, c);
		possibleMoves.insert(possibleMoves.end(), pieceMoves.begin(), pieceMoves.end());
	}

	for (MCTSNode* n : node->getChildren()) {
		possibleMoves.erase(remove_if(possibleMoves.begin(), possibleMoves.end(),
			[n](const Move& mv) {
				return mv.fromRow == n->getMove().fromRow &&
					mv.fromCol == n->getMove().fromCol &&
					mv.toRow == n->getMove().toRow &&
					mv.toCol == n->getMove().toCol;
			}), possibleMoves.end());
	}

	if (possibleMoves.size() == 1) {
		node->setFullyExpanded();
	}

	int randIndex = std::rand() % possibleMoves.size();
	MCTSNode* newNode = new MCTSNode(node, possibleMoves[randIndex], tempGame.getCurrentTurn());
	node->newChild(newNode);
	tempGame.applyMove(possibleMoves[randIndex]);

	return newNode;
}

bool MCTSAgent::simulateRandomGame(Game game) {

	auto start = high_resolution_clock::now(); // kezdőidő

	mt19937 rng(random_device{}());
	string reason;

	for (int i = 0; i < 1000; i++) {
		vector<Move> allMoves;
		uint64_t bb = (game.getCurrentTurn() == Color::White) ? game.getWhitePieces() : game.getBlackPieces();
		int numPieces = 0;
		while (bb) {
			int sq = __builtin_ctzll(bb);	// legalsó beállított bit indexe
			bb &= (bb - 1);					// töröljük a feldolgozott bitet
			numPieces++;
			int r = sq / 8;
			int c = sq % 8;
			auto pieceMoves = game.getLegalMoves(r, c);
			allMoves.insert(allMoves.end(), pieceMoves.begin(), pieceMoves.end());
		}
		if (numPieces == 1) {
			reason = "only_king_left";
			auto end = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(end - start).count();
			Logger::debug(std::to_string(i) + " " + (game.getCurrentTurn() == Color::White ? "white" : "black") + " " + reason + " | time=" + std::to_string(duration) + "ms");
			return game.getCurrentTurn() == Color::White ? false : true;
		} // csak a király maradt

		if (allMoves.empty()) {
			if (game.isCheckmate()) {
				reason = "checkmate";
			}
			else if (game.isStalemate()) {
				reason = "stalemate";
			}
			auto end = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(end - start).count();
			Logger::debug(std::to_string(i) + " " + (game.getCurrentTurn() == Color::White ? "white" : "black") + " " + reason + " | time=" + std::to_string(duration) + "ms");
			return reason == "checkmate" ? (game.getCurrentTurn() == Color::White ? false : true) : true; // patt döntetlen
		}

		uniform_int_distribution<int> dist(0, static_cast<int>(allMoves.size()) - 1);
		int chosenIndex = dist(rng);

		game.applyMove(allMoves[chosenIndex]);
	}
	return game.getCurrentTurn() == Color::White ? true : false;
}

void MCTSAgent::backpropagation(MCTSNode* node, bool win) {
	while (node != nullptr)
	{
		bool nodeCol = node->getPlayer() == Color::White;
		if (nodeCol == win) {
			node->addWin(1.0f);

		}
		else {
			node->addWin(0.0f);
		}
		node->addVisit();

		node = node->getParent();
	}
}

void MCTSAgent::computeMCTSTree(int iterations) {
	for (int i = 0; i < iterations; i++) {
		tempGame = game;
		MCTSNode* selectedNode = selection();
		MCTSNode* expandedNode = expansion(selectedNode);
		bool win = simulateRandomGame(tempGame);
		backpropagation(expandedNode, win);
	}
}
