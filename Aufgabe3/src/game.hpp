//
// Created by Theodor Teslia on 03.10.20.
// File with all classes that simulate the competitions
// class Turnier as Base class
// Every other class derives its classes from there
//

#ifndef AUFGABE3_GAME_HPP
#define AUFGABE3_GAME_HPP

#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include "player.hpp"

class Turnier {
protected:
	std::set<Player> players;

public:
	void add(Player pl);
	virtual int play() = 0;
};


class Liga : public Turnier {
private:
	std::map<int, int> scores;
	void initializeScores();
	static int playRound(Player& ply1, Player& ply2);
public:
	Liga();
	Liga(std::vector<Player> plyVec);
	int play();
};


class KO : public Turnier {
protected:
	enum concretePlan {
		WORSTVSBEST,
		ORDER,
		RANDOM,
		AMOUNT
	};

	std::vector<Player> plyVec;

	void fillPlayerVec();
	void organizeThePairs(concretePlan plan);
	static Player& playRound(Player& ply1, Player& ply2);

public:
	KO();
	KO(std::vector<Player> plyVec);
	int play();
};


class KOx5 : public KO{
private:
	static Player& playRound(Player& ply1, Player& ply2);
public:
	KOx5();
	KOx5(std::vector<Player> plyVec);
	int play();
};

#endif //AUFGABE3_GAME_HPP
