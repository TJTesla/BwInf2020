//
// Created by Theodor Teslia on 18.11.20.
//

#ifndef AUFGABE5_WISH_H
#define AUFGABE5_WISH_H

#include <set>
#include "Player.h"

class Wish {
private:
	int wish;
	int wishAmount;
	std::set<Player*> plys;
public:
	Wish();

	void increaseWishAmount();
	void setWish(int pWish);
	int getWish() const;
	void newPlayer(Player* ply);
	std::size_t getAmountPlayer();
	bool has(Player*& ply);
};


#endif //AUFGABE5_WISH_H
