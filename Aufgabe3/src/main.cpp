#include <iostream>
#include <fstream>
#include <string>

#include "game.hpp"

int highestNumber(std::vector<Player>& vec) {
	Player bestPlayer = vec.at(0);
	for (auto& i : vec) {
		if (i.getStrength() > bestPlayer.getStrength()) {
			bestPlayer = i;
		}
	}
	return bestPlayer.getNumber();
}

const int ITERATIONS = 100000;  // Controls how many "games" are played to get the average

int main(int argc, char** argv) {

	std::string path;  // Get the correct file; Given by the command line argument or the default
	if (argc == 1) {
		path = "../src/spielstaerken1.txt";
	} else {
		path = "../src/" + (std::string)argv[1];
	}
	std::ifstream txtFile(path);
	if (!txtFile)
		return 1;

	std::cout << "Liga: 0 \nK.O.: 1 \nK.O.x5: 2" << std::endl;  // Decision which competition should be executed
	char input = '2';
	std::cin >> input;
	if (input != '0' && input != '1' && input != '2') {
		input = '0';
	}

	bool sorting;  // Should the players in a game be sorted by their number or strength
	if (input == '0')
		sorting = false;
	else
		sorting = true;

	std::vector<Player> plyField;  // Stores every player

	std::string strAmount;  std::getline(txtFile, strAmount);
	int amount = std::stoi(strAmount);  // Amount of players

	std::string line;
	for (int i = 0; i < amount; i++) {  // Fill the storages with the Players
		std::getline(txtFile, line);
		int curStrength = std::stoi(line);

		Player tempPlayer(curStrength, i, sorting);
		plyField.push_back(tempPlayer);
	}

	int bestPlayerNumber = highestNumber(plyField);  // For comparison with a gamewinner

	int amountBestPlayerWon = 0;

	switch (input) {  // Only differences in cases is object-type of playing object
		case '0': {  // Liga
			std::cout << "The competition-form league is being played." << std::endl;
			Liga form(plyField);
			for (int i = 0; i < ITERATIONS; i++) {
				int curWinner = form.play();
				if (curWinner == bestPlayerNumber) {
					amountBestPlayerWon++;
				}
			}
		} break;

		case '1': {  // KO
			std::cout << "The competition-form K.O. is being played." << std::endl;
			KO form(plyField);
			for (int i = 0; i < ITERATIONS; i++) {
				int curWinner = form.play();
				if(curWinner == bestPlayerNumber) {
					amountBestPlayerWon++;
				}
			}
		} break;

		case '2': {  // KOx5
			std::cout << "The competition-form K.O.x5 is being played." << std::endl;
			KOx5 form(plyField);
			for (int i = 0; i < ITERATIONS; i++) {
				int curWinner = form.play();
				if(curWinner == bestPlayerNumber) {
					amountBestPlayerWon++;
				}
			}
		} break;

		default:
			break;
	}

	double winrate = amountBestPlayerWon/(double)ITERATIONS * 100;  // Percentage of wins of the best player
	std::cout << "The best player won with a winrate of " /*<< std::setprecision(5)*/ << winrate;
	std::cout << "%." << std::endl;

	return 0;
}
