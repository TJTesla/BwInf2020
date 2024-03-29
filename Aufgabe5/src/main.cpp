#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <string>
#include <map>

#include "Wish.h"
#include "Player.h"

#define GET std::get

template <typename T1, typename T2>
bool isIn(T1& storage, T2 variable) {
	return std::find(storage.begin(), storage.end(), variable) != storage.end();
}

int fittingWish(Player*& ply, std::vector<Wish>& wishes);
Player* findPlayerWithLowestWishes(std::vector< std::pair<Player*, int> >& vec, std::vector<Wish>& list, std::map<int, int>& used, bool& specialSecond, std::set<int>& notToBeUsedAgain);
int whichWishToBeUsed(Player*& ply, std::vector<Wish>& list, std::set<int>& used, bool state, bool specialSecond, std::vector< std::pair<Player*, int> >& plyList);
int getUnusedWish(int amount, std::set<int>& used);
bool find(std::vector<Wish>& vec, int val);

void printResults(std::vector<Player>& vec, std::ofstream& outFile);

int main(int argc, char* argv[]) {
	std::string path;
	std::string defaultFile = "wichteln1.txt";
	if (argc == 1) {
		path = "../examples/"+defaultFile;
	} else {
		path = "../examples/" + (std::string)argv[1];
	}
	std::ifstream txtFile(path);
	if (!txtFile) return 1;

	path = "";
	if (argc == 1) {
		path = "../out/"+defaultFile;
	} else {
		path = "../out/" + (std::string)argv[1];
	}
	std::ofstream outFile(path);
	if (!outFile) return 1;

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
	std::map<int, int> alreadyUsed;
	std::set<int> usedWishes;


	for (int i = 1; i <= amount; i++) {
		alreadyUsed.insert(std::pair<int, int>(i, 0));
	}
	for (auto& i : plyVec) {  // Fill first wishes
		wishes.at(FIRST).insert(i.getWish(FIRST));
		alreadyUsed.at(i.getWish(FIRST)) += 1;
	}
	for (auto& i : plyVec) {  // Fill second wishes
		if ( wishes.at(FIRST).find(i.getWish(SECOND)) == wishes.at(FIRST).end() &&  // Number doesn't occur in higher List
				alreadyUsed.at(i.getWish(FIRST)) != 1) {
			wishes.at(SECOND).insert(i.getWish(SECOND));  // ^ False if not clearly assigned
			alreadyUsed.at(i.getWish(SECOND)) += 1;
		}
	}
	for (auto& i : plyVec) {  // Fill third wishes
		if (wishes.at(FIRST).find(i.getWish(THIRD)) == wishes.at(FIRST).end() &&
				wishes.at(SECOND).find(i.getWish(THIRD)) == wishes.at(SECOND).end() &&
				alreadyUsed.at(i.getWish(FIRST)) != 1) {
			wishes.at(THIRD).insert(i.getWish(THIRD));
			alreadyUsed.at(i.getWish(THIRD)) += 1;
		}
	}

	std::unordered_set<int>::iterator firstWishIterator;
	for (firstWishIterator = wishes.at(FIRST).begin(); firstWishIterator != wishes.at(FIRST).end(); firstWishIterator++) {
		std::vector< std::pair<Player*, int> > currentGroup;

		for (auto& i : plyVec) {  // Get group with same first wish
			if (i.getWish(FIRST) == *firstWishIterator) {
				currentGroup.emplace_back(&i, -1);
			}
		}
		if (currentGroup.size() == 1) {  // If only one player w/ this first wish -> Assign it to him
			currentGroup.at(0).first->setPresent(currentGroup.at(0).first->getWish(FIRST));
			usedWishes.insert(currentGroup.at(0).first->getWish(FIRST));
			continue;
		}

		std::set< std::tuple<int, int, WishEnum> > possiblePresents;  // Wish, Playerposition in curGroup, Wishposition
		for (int i = 0; i < currentGroup.size(); i++) {  // Fill all possible presents
			Player curPly = *(currentGroup.at(i).first);
			for (int j = 0; j < 3; j++) {
				if ( isIn(wishes.at(j), curPly.getWish( (WishEnum)j )) && !isIn(usedWishes, curPly.getWish( (WishEnum)j ))) {
					possiblePresents.insert(std::tuple<int, int, WishEnum>(curPly.getWish((WishEnum)j), i, (WishEnum)j));
				}
			}
		}
		if (possiblePresents.empty()) {  // If no wish is possible, continue with next first wish
			continue;
		}
		if (possiblePresents.size() == 1) {  // If only one wish is possible, assign it to possible Player and continue
			std::tuple<int, int, WishEnum> firstObj = *(possiblePresents.begin());
			currentGroup.at(GET<1>(firstObj)).first->setPresent(GET<0>(firstObj));
			continue;
		}

		std::vector<Wish> wishList;  // List with the wishes, the amount they are wished and the players who wish it
		int counter = 0;
		for (auto& i : possiblePresents) {
			if (find(wishList, GET<0>(i))) {
				continue;
			}
			wishList.emplace_back();
			for (auto& j : currentGroup) {
				if (j.first->getWish(GET<2>(i)) == GET<0>(i)) {
					wishList.at(counter).setWish(GET<0>(i));
					wishList.at(counter).increaseWishAmount();
					wishList.at(counter).newPlayer(j.first);
				}
			}
			counter++;
		}

		for (auto& i : currentGroup) {  // Fill how often a possible present occurs in wishList
			int fitting = fittingWish(i.first, wishList);
			i.second = fitting;
		}

		bool firstWasGiven = false;
		// Main Algorithm:
		for (int i = 0; i < currentGroup.size(); i++) {  // Iterate once for every player
			bool specialSecondWasTaken = false;
			Player* tempPlayer = findPlayerWithLowestWishes(currentGroup, wishList, alreadyUsed, specialSecondWasTaken, usedWishes);  // Find out whose wishes occur the least
			int wishToBeUsed = whichWishToBeUsed(tempPlayer, wishList, usedWishes, firstWasGiven, specialSecondWasTaken, currentGroup);  // Find out which present he gets
			tempPlayer->gotAssigned();
			firstWasGiven = true;
			if (wishToBeUsed == -1 || tempPlayer->getPresent() != -1) {  // Error prevention
				continue;
			}
			tempPlayer->setPresent(wishToBeUsed);  // Setting present
			usedWishes.insert(wishToBeUsed);
		}

		/*std::cout << currentGroup.at(0).first->getWish(FIRST) << ": ";
		printResults(plyVec); */
	}

	for (auto& i : plyVec) {
		if (i.getPresent() != -1) {
			continue;
		}
		int newWish = getUnusedWish(amount, usedWishes);
		if (newWish == -1) {
			break;
		} else {
			i.setPresent(newWish);
			usedWishes.insert(newWish);
		}
	}

	printResults(plyVec, outFile);

	return 0;  // End of program
}



int fittingWish(Player*& ply, std::vector<Wish>& wishes) {
	int counter = 0;
	for (auto& i : wishes) {
		if (i.getWish() == ply->getWish(FIRST)) {
			counter += 1;
		} else if (i.getWish() == ply->getWish(SECOND)) {
			counter += 2;
		} else if (i.getWish() == ply->getWish(THIRD)) {
			counter += 3;
		}
	}
	return counter;
}

Player* findPlayerWithLowestWishes(std::vector< std::pair<Player*, int> >& vec, std::vector<Wish>& list, std::map<int, int>& used, bool& specialSecond, std::set<int>& notToBeUsedAgain) {
	/*for (auto& i : list) {
		if (used.at(i.getWish()) == 1 && i.isPosition(SECOND)) {  // If there is a wish that only appears once and is a second wish
			std::cout << "Special Second" << std::endl;
			specialSecond = true;
			return i.withPosition(SECOND); // Return the Player with that wish
		}
	} */

	std::pair<Player*, int> lowestPair;  // Set up the starting player
	int counter = 0;  // Starting player is not allowed to have already a fulfilled present
	do {  // Else this leads to error
		lowestPair.first = vec.at(counter).first;
		lowestPair.second = vec.at(counter).second;
		counter++;
	} while (lowestPair.first->wasAssigned() && counter <= vec.size());

	Player* lowestPlayer = lowestPair.first;
	int lowestAmount = lowestPair.second;
	for (auto& i : vec) {  // Find person with least wishes fulfillable and who didn't get a present yet
		if (i.second < lowestAmount && i.second != 0 && i.first->getPresent() == -1 && !lowestPlayer->wasAssigned()) {
			lowestAmount = i.second;
			lowestPlayer = i.first;
		}
	}
	if (used.at(lowestPlayer->getWish(SECOND)) == 1 && notToBeUsedAgain.find(lowestPlayer->getWish(SECOND)) == notToBeUsedAgain.end() && !lowestPlayer->wasAssigned() && find(list, lowestPlayer->getWish(SECOND))) {  // TODO: Do something with the special second
		specialSecond = true;
	}
	return lowestPlayer;
}

int whichWishToBeUsed(Player*& ply, std::vector<Wish>& list, std::set<int>& used, bool state, bool specialSecond, std::vector< std::pair<Player*, int> >& plyList) {  // TODO: Wooork!
	if (specialSecond && state) {
		return ply->getWish(SECOND);
	}
	if (!state) {
		return ply->getWish(FIRST);
	}

	for (auto& i : list) {  // Use a fitting second wish
		if (used.find(i.getWish()) == used.end() && i.getWish() == ply->getWish(SECOND)) {
			return i.getWish();
		}
	}
	for (auto& i : list) {  // Use a fitting third wish
		if (used.find(i.getWish()) == used.end() && i.getWish() == ply->getWish(THIRD)) {
			return i.getWish();
		}
	}

	return -1;
}

int getUnusedWish(int amount, std::set<int>& used) {
	for (int i = 1; i <= amount; i++) {  // Find the next present that hasn't been gifted yet
		if (used.find(i) == used.end()) {  // Present is not allowed to be in used Array
			return i;
		}
	}
	return -1;
}

void printResults(std::vector<Player>& vec, std::ofstream& outFile) {
	std::tuple<int, int, int> counter;
	int playerNum = 1;
	for (auto& i : vec) {
		int gift = i.getPresent();
		std::string wishNum = "dem ";
		if (gift == i.getWish(FIRST)) { wishNum += "1."; GET<0>(counter) += 1; }
		else if (gift == i.getWish(SECOND)) { wishNum += "2."; GET<1>(counter) += 1; }
		else if (gift == i.getWish(THIRD)) { wishNum += "3."; GET<2>(counter) += 1; }
		else wishNum = "keinem";
		outFile << "Der " << playerNum << ". Teilnehmer bekommt das Geschenk " << i.getPresent();
		outFile << " (Dies entspricht " << wishNum << " Wunsch)" << std::endl;
		playerNum++;
	}
	std::cout << "Es wurden" << std::endl;
	std::cout << "'"<< GET<0>(counter) << "' 1. Wünsch(e)" << std::endl;
	std::cout << "'"<< GET<1>(counter) << "' 2. Wünsch(e) und" << std::endl;
	std::cout << "'"<< GET<2>(counter) << "' 3. Wünsch(e) erfüllt" << std::endl;
	//std::cout << "1.: " << GET<0>(counter) << "; 2.: " << GET<1>(counter) << "; 3.: " << GET<2>(counter);
}

bool find(std::vector<Wish>& vec, int val) {  // If val is in vec -> return true; Otherwise false
	for (auto& i : vec) {
		if (i.getWish() == val) {
			return true;
		}
	}
	return false;
}