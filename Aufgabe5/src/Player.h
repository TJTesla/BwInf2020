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
	bool assigned;
public:
	Player();
	explicit Player(std::array<int, 3> wishes);
	[[nodiscard]] int getWish(WishEnum number) const;
	void setPresent(int number);
	[[nodiscard]] int getPresent() const;
	void gotAssigned();
	[[nodiscard]] bool wasAssigned() const;

	bool operator< (const Player& ply) const;
	bool operator==(const Player& ply) const;
};

#endif //AUFGABE5_PLAYER_H
