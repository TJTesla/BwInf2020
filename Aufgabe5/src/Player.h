//
// Created by Theodor Teslia on 07.11.20.
//

#ifndef AUFGABE5_PLAYER_H
#define AUFGABE5_PLAYER_H

#include <array>

enum WishEnum {
	FIRST,
	SECOND,
	THIRD
};

class Player {
private:
	std::array<int, 3> wishes{};
	int present;
public:
	Player();
	explicit Player(std::array<int, 3> wishes);
	int getWish(WishEnum number) const;
	void setPresent(int number);
	int getPresent() const;

	bool operator< (const Player& ply) const;
	bool operator==(const Player& ply) const;
};

#endif //AUFGABE5_PLAYER_H
