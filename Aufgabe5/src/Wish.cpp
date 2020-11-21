//
// Created by Theodor Teslia on 18.11.20.
//

#include "Wish.h"

Wish::Wish() {
	this->wish = -1;
	this->wishAmount = 0;
}

void Wish::increaseWishAmount() {
	this->wishAmount++;
}

void Wish::setWish(int pWish) {
	this->wish = pWish;
}

int Wish::getWish() const {
	return this->wish;
}

void Wish::newPlayer(Player* ply) {
	this->plys.insert(ply);
}

std::size_t Wish::getAmountPlayer() {
	return this->plys.size();
}

bool Wish::has(Player*& ply) {
	return std::find(plys.begin(), plys.end(), ply) != plys.end();
}
