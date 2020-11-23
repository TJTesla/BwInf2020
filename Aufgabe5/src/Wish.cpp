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

Player* Wish::getFirstPlayer() {
	try {
		return *(this->plys.begin());
	} catch (std::exception& e) {
		return nullptr;
	}
}

bool Wish::isPosition(WishEnum pos) {
	bool returnVal = false;
	for (auto& i : this->plys) {
		if (i->getWish(pos) == this->wish) {
			returnVal = true;
			break;
		}
	}
	return returnVal;
}

Player* Wish::withPosition(WishEnum pos) {
	for (auto& i : this->plys) {
		if (i->getWish(pos) == this->wish) {
			return i;
		}
	}
	return this->getFirstPlayer();
}