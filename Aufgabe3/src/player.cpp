//
// Created by Theodor Teslia on 03.10.20.
//

#include "player.hpp"

Player::Player(int pStaerke, int spielerNummer, bool pSortByStrength) {
	this->strength = pStaerke;
	this->playerNumber = spielerNummer;
	this->sortByStrength = pSortByStrength;
}

Player::~Player() {
	this->strength = 0;
	this->playerNumber = 0;
}

bool Player::operator<(const Player &ply) const {
	if (sortByStrength) {
		return this->strength < ply.strength;
	} else {
		return this->playerNumber < ply.playerNumber;
	}
}
