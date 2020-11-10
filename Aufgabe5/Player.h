//
// Created by Theodor Teslia on 07.11.20.
//

#ifndef AUFGABE5_PLAYER_H
#define AUFGABE5_PLAYER_H

#include <array>

enum Wish {
	FIRST,
	SECOND,
	THIRD
};

class Player {
private:
	std::array<int, 3> wishes{};
public:
	explicit Player(std::array<int, 3> wishes);
	int getWish(Wish number);
};

#endif //AUFGABE5_PLAYER_H
