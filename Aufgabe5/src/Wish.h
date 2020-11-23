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
	[[nodiscard]] int getWish() const;
	void newPlayer(Player* ply);
	std::size_t getAmountPlayer();
	bool has(Player*& ply);
	Player* getFirstPlayer();
	bool isPosition(WishEnum pos);
	Player* withPosition(WishEnum pos);
};


#endif //AUFGABE5_WISH_H
