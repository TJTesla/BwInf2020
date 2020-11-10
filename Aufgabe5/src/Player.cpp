//
// Created by Theodor Teslia on 07.11.20.
//

#include "Player.h"

Player::Player(std::array<int, 3> wishes) {
	this->wishes = wishes;
}

int Player::getWish(Wish number) {
	return wishes.at(number);
}
