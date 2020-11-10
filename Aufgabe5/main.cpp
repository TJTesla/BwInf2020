#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>

#include "Player.h"

template <typename T1, typename T2>
bool isIn(T1 storage, T2 variable) {
	return storage.find(variable) != storage.end();
}

int main(int argc, char* argv[]) {
	std::string path;
	if (argc == 1) {
		path = "../examples/wichteln1.txt";
	} else {
		path = "../examples/" + (std::string)argv[1];
	}
	std::ifstream txtFile(path);
	if (!txtFile) return 1;

	std::string fileLineStorage;
	std::getline(txtFile, fileLineStorage);
	int amount = std::stoi(fileLineStorage);

	std::vector<Player> plyVec;

	for (int i = 0; i < amount; i++) {
		std::array<int, 3> values = {0, 0, 0};
		for (int j = 0; j < 3; j++) {
			txtFile >> fileLineStorage;
			values.at(j) = std::stoi(fileLineStorage);
		}
		Player tempPlayer(values);
		plyVec.push_back(tempPlayer);
	}

	std::array< std::unordered_set<int>, 3 > wishes;

	for (auto& i : plyVec) {
		wishes.at(FIRST).insert(i.getWish(FIRST));
	}
	for (auto& i : plyVec) {
		if ( !isIn(wishes.at(FIRST), i.getWish(FIRST)) ) {
			wishes.at(SECOND).insert(i.getWish(SECOND));
		}
	}
	for (auto& i : plyVec) {
		if ( !isIn(wishes.at(FIRST), i.getWish(FIRST)) &&
		     !isIn(wishes.at(SECOND), i.getWish(SECOND)) ) {
			wishes.at(THIRD).insert(i.getWish(THIRD));
		}
	}

	auto firstWishIterator = wishes.at(FIRST).begin();
	for (firstWishIterator = wishes.at(FIRST).begin(); firstWishIterator != wishes.at(FIRST).end(); firstWishIterator++) {
		std::vector<Player> currentGroup;
		for (auto& i : plyVec) {
			if (i.getWish(FIRST) == *firstWishIterator) {
				currentGroup.push_back(i);
			}
		}
		// Get all presents that are possible / Would fulfill a wish
		// Decide who gets what present
		// TODO: Work on the algorithm that determines who gets what
	}

	return 0;
}
