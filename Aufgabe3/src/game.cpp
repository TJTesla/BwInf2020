#include "game.hpp"

void Turnier::add(Player pl) {
	this->players.insert(pl);
}

void Liga::initializeScores() {
	this->scores.clear();
	for (auto i = players.begin(); i != players.end(); i++) {
		this->scores[i->getNumber()] = 0;
	}
}

int Liga::playRound(Player& ply1, Player& ply2) {  // Simulates a match of two players
	int rndNum = rand() % (ply1.getStrength() + ply2.getStrength());
	if (rndNum >= ply1.getStrength()) {
		return ply2.getNumber();  // Return playernumber
	} else {
		return ply1.getNumber();
	}
}

Liga::Liga() { srand((unsigned)time(0)); }
Liga::Liga(std::vector<Player> plyVec) {
	srand((unsigned)time(0));
	for (auto i : plyVec) {  // Fill the set which saves the players
		this->players.insert(i);
	}
	this->initializeScores();
}

int Liga::play() {
	this->initializeScores();

	for (std::set<Player>::iterator i = players.begin(); i != players.end(); i++) {  // Everyone "fights" against everyone
		for (std::set<Player>::iterator j = std::next(i, 1); j != players.end(); j++) {
			int winningPlayer = Liga::playRound((Player&)*i, (Player&)*j);
			this->scores[winningPlayer] = scores.at(winningPlayer) + 1;
		}
	}

	std::pair<int, int> bestPlayer(scores.begin()->first, scores.begin()->second);
	for (std::map<int, int>::iterator i = scores.begin(); i != scores.end(); i++) {
		if (i->second > bestPlayer.second) {
			bestPlayer.first = i->first;
			bestPlayer.second = i->second;
		}
	}  // Determine the best Player by saving the current best Player and comparing to the next Player

	this->scores.clear();
	return bestPlayer.first;  // return playernumber of winning player
}

KO::KO() { srand((unsigned)time(0)); }
KO::KO(std::vector<Player> plyVec) {
	for (auto i : plyVec) {
		this->players.insert(i);  // Fill both containers with the players
		this->plyVec.push_back(i);
	}
	srand((unsigned)time(0));
}

Player& KO::playRound(Player &ply1, Player &ply2) {  // Simulates a match between two players
	int rndNum = rand() % (ply1.getStrength() + ply2.getStrength());
	if (rndNum >= ply1.getStrength()) {
		return ply2;  // Returns reference to player
	} else {
		return ply1;
	}
}

void KO::fillPlayerVec() {
	plyVec.clear();
	for (auto i = players.begin(); i != players.end(); i++) {
		plyVec.push_back(*i);
	}
}

void KO::organizeThePairs(concretePlan plan) {
	if (plan == ORDER) {  // Players are sorted by increasníng strength
		return;
	} else if (plan == RANDOM) {  // The players are randomly distributed
		auto rd = std::random_device {};
		auto rng = std::default_random_engine { rd() };
		std::shuffle(plyVec.begin(), plyVec.end(), rng);
	} else if (plan == WORSTVSBEST) {  // The worst plays against the best, the 2. best against the 2. worst and so on
		plyVec.clear();
		auto reverse = std::prev(players.end(), 1);  // Iterator from front and back
		auto normal = players.begin();
		for (auto i = 0; i < players.size()/2; i++, normal++, reverse--) {
			plyVec.push_back(*normal);
			plyVec.push_back(*reverse);
		}
	}
}

int KO::play() {  // Play a tournament
	fillPlayerVec();
	std::vector<Player> winnerBank;

	organizeThePairs(ORDER);

	while (plyVec.size() > 1) {  // Loop until the winner is determined
		std::size_t formerSize;

		formerSize = plyVec.size();
		for (int i = 0; i < formerSize/2; i++) {  // Two players lay against each other, better one is saved in second arr;
			Player winningPlayer = KO::playRound(plyVec.at(0), plyVec.at(1));
			plyVec.erase(plyVec.begin(), plyVec.begin() + 2);
			winnerBank.push_back(winningPlayer);
		}  // Both elements are deleted afterwards

		plyVec = winnerBank;
		winnerBank.clear();
	}

	return plyVec.at(0).getNumber();  // Return the last remaining player's number
}

KOx5::KOx5() { srand((unsigned)time(0)); }
KOx5::KOx5(std::vector<Player> plyVec) {
	for (auto i : plyVec) {
		this->players.insert(i);  // Fill both containers with the players
		this->plyVec.push_back(i);
	}
	srand((unsigned)time(0));
}

Player& KOx5::playRound(Player &ply1, Player &ply2) {
	std::pair<int, int> scores(0, 0);
	for (int i = 0; i < 5; i++) {  // Save who wins a match over 5 iterations
		int rndNum = rand() % (ply1.getStrength() + ply2.getStrength());
		if (rndNum >= ply1.getStrength()) {
			scores.second++;
		} else {
			scores.first++;
		}
	}
	if (scores.second > scores.first) {  // Return the player who one mor matches out of 5
		return ply2;
	} else {
		return ply1;
	}
}

int KOx5::play() {  // Play a tournament
	fillPlayerVec();
	std::vector<Player> winnerBank;

	organizeThePairs(WORSTVSBEST);

	while (plyVec.size() > 1) {  // Loop until the winner is determined
		std::size_t formerSize;

		formerSize = plyVec.size();
		for (int i = 0; i < formerSize/2; i++) {  // Two players lay against each other, better one is saved in second arr;
			Player winningPlayer = KOx5::playRound(plyVec.at(0), plyVec.at(1));
			plyVec.erase(plyVec.begin(), plyVec.begin() + 2);
			winnerBank.push_back(winningPlayer);
		}  // Both elements are deleted afterwards

		plyVec = winnerBank;
		winnerBank.clear();
	}

	return plyVec.at(0).getNumber();  // Return the last remaining player's number
}
