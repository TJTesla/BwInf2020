//
// Created by Theodor Teslia on 07.11.20.
//

#include "Player.h"

Player::Player() {
	this->wishes.at(0) = -1;
	this->wishes.at(1) = -1;
	this->wishes.at(2) = -1;
	this->present = -1;
}

Player::Player(std::array<int, 3> wishes) {
	this->wishes = wishes;
	this->present = -1;
}

int Player::getWish(WishEnum number) const {
	return wishes.at(number);
}

void Player::setPresent(int number) {
	this->present = number;
}

int Player::getPresent() const {
	return this->present;
}

bool Player::operator<(const Player &ply) const {
	bool returnVal;
	returnVal = this->getWish(FIRST) != ply.getWish(FIRST) &&
			this->getWish(SECOND) != ply.getWish(SECOND) &&
			this->getWish(THIRD) != ply.getWish(THIRD);
	return returnVal;
}

bool Player::operator==(const Player &ply) const {
	return this->operator<(ply);
}