//
// Created by Theodor Teslia on 03.10.20.
// Saves one Player
//

#ifndef AUFGABE3_PLAYER_HPP
#define AUFGABE3_PLAYER_HPP

class Player {
private:
	int strength;
	int playerNumber;

	bool sortByStrength;
public:
	explicit Player(int staerke, int spielerNummer, bool pSortByStrength=true);
	~Player();

	bool operator< (const Player& ply) const;

	int getStrength() const { return this->strength; }
	int getNumber() const { return this->playerNumber; }
};

#endif //AUFGABE3_PLAYER_HPP
