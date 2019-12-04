#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include <unordered_map>
#include <random>

enum class RPSChoice {
	ROCK,
	PAPER,
	SCISSORS
};

std::vector<RPSChoice> playerChoices;
int yourWins;
int aiWins;
int totalGames;
const int N_GRAM_THRESHOLD = 3;

std::string RPSChoiceToString(RPSChoice choice) {
	switch (choice) {
	case RPSChoice::ROCK:
		return "ROCK";
	case RPSChoice::PAPER:
		return "PAPER";
	case RPSChoice::SCISSORS:
		return "SCISSORS";
	}
	throw std::exception("Unhandled RPSChoice.");
}

RPSChoice winningOption(RPSChoice choice) {
	switch (choice) {
	case RPSChoice::ROCK:
		return RPSChoice::PAPER;
	case RPSChoice::PAPER:
		return RPSChoice::SCISSORS;
	case RPSChoice::SCISSORS:
		return RPSChoice::ROCK;
	}
	throw std::exception("Unhandled RPSChoice.");
}

int rpsWinner(RPSChoice lhs, RPSChoice rhs) {
	if (lhs == RPSChoice::ROCK) {
		if (rhs == RPSChoice::ROCK) {
			return 0;
		}
		else if (rhs == RPSChoice::PAPER) {
			return -1;
		}
		else if (rhs == RPSChoice::SCISSORS) {
			return 1;
		}
	}
	else if (lhs == RPSChoice::PAPER) {
		if (rhs == RPSChoice::ROCK) {
			return 1;
		}
		else if (rhs == RPSChoice::PAPER) {
			return 0;
		}
		else if (rhs == RPSChoice::SCISSORS) {
			return -1;
		}
	}
	else if (lhs == RPSChoice::SCISSORS) {
		if (rhs == RPSChoice::ROCK) {
			return -1;
		}
		else if (rhs == RPSChoice::PAPER) {
			return 1;
		}
		else if (rhs == RPSChoice::SCISSORS) {
			return 0;
		}
	}
	throw std::exception("Unhandled RPSChoice.");
}

void printOptions() {
	std::cout << "Your options:" << std::endl;
	std::cout << "Rock - R/r" << std::endl;
	std::cout << "Paper - P/p" << std::endl;
	std::cout << "Scissors - S/s" << std::endl;
	std::cout << "Win rates - H/h" << std::endl;
	std::cout << "Quit - Q/q" << std::endl;
}

RPSChoice getOption(std::string line) {
	std::cout << "Your input: " << line << std::endl;
	std::for_each(line.begin(), line.end(), [](char& c) {
		c = ::tolower(c);
		});
	if (line == "r") {
		return RPSChoice::ROCK;
	}
	else if (line == "p") {
		return RPSChoice::PAPER;
	}
	else if (line == "s") {
		return RPSChoice::SCISSORS;
	}
	else if (line == "q") {
		throw std::exception("Quit called.");
	}
	else if (line == "h") {
		std::cout << "Your wins: " << yourWins << std::endl;
		std::cout << "AI wins: " << aiWins << std::endl;
		std::cout << "AI win rate: " << (aiWins / totalGames) << std::endl;
	}

	throw std::exception("Unhandled option.");
}

RPSChoice predictChoice() {
	std::unordered_map<std::string, int> probability;

	static std::string choices[9] = {
		"RR", "RP", "RS",
		"PR", "PP", "PS",
		"SR", "SP", "SS"
	};

	for (auto c : choices) {
		probability[c] = 0;
	}

	if (playerChoices.size() < N_GRAM_THRESHOLD) {
		static RPSChoice randomChoices[3] = {
			RPSChoice::ROCK,
			RPSChoice::PAPER,
			RPSChoice::SCISSORS
		};
		// random option
		int r = rand() % 3;
		return randomChoices[r];
	}
	else {

		RPSChoice highestChance = RPSChoice::ROCK;
		int highestCount = probability[RPSChoice::ROCK];
		for (auto& it : probability) {
			if (it.second > highestCount) {
				highestChance = it.first;
				highestCount = it.second;
			}
		}
		std::cout << "Highest probability: " << RPSChoiceToString(highestChance) << std::endl;
		return highestChance;
	}
}

int main() {
	playerChoices = std::vector<RPSChoice>();

	bool keepRunning = true;
	std::string line;
	yourWins = 0;
	aiWins = 0;
	totalGames = 0;

	while (keepRunning) {
		try {
			printOptions();
			RPSChoice aiChoice = winningOption(predictChoice());
			std::getline(std::cin, line);
			playerChoices.push_back(getOption(line));

			std::cout << "Your choice: " << RPSChoiceToString(playerChoices.back()) << std::endl;
			std::cout << "AI choice: " << RPSChoiceToString(aiChoice) << std::endl;

			int playerWon = rpsWinner(playerChoices.back(), aiChoice);
			if (playerWon == 1) {
				std::cout << "You win." << std::endl;
				yourWins++;
			}
			else if (playerWon == 0) {
				std::cout << "Tie." << std::endl;
			}
			else if (playerWon == -1) {
				std::cout << "You lose." << std::endl;
				aiWins++;
			}
			totalGames++;
#if DEBUG
			for (auto option : playerChoices) {
				switch (option) {
				case RPSChoice::ROCK:
					std::cout << "ROCK";
					break;
				case RPSChoice::PAPER:
					std::cout << "PAPER";
					break;
				case RPSChoice::SCISSORS:
					std::cout << "SCISSORS";
					break;
				}
				std::cout << ",";
			}
			std::cout << std::endl;
#endif
		}
		catch (std::exception & ex) {
			std::cout << ex.what() << std::endl;
			if (std::strcmp(ex.what(), "Quit called.") == 0) {
				keepRunning = false;
			}
		}
	}

	return 0;
}