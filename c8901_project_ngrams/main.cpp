#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <exception>
#include <map>
#include <random>

enum class RPSChoice {
	ROCK,
	PAPER,
	SCISSORS
};

struct KeyDataRecord {
public:
	// Holds the counts for each successor action
	std::map<RPSChoice, int> counts;

	// Holds the total number of times the window has been seen
	int total;

	KeyDataRecord() : total(0) {
		
	}
};

// Holds the frequency data
std::map<std::vector<RPSChoice>, KeyDataRecord> data;
int yourWins;
int aiWins;
int totalGames;
const int WINDOW_SIZE = 2;
// Holds the size of the window + 1
const int N = WINDOW_SIZE + 1;

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

RPSChoice randomChoice() {
	static RPSChoice choices[3] = {
		RPSChoice::ROCK,
		RPSChoice::PAPER,
		RPSChoice::SCISSORS
	};

	return choices[rand() % 3];
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

/**
* Registers a set of actions with predictor, updating
* its data. We assume actions has exactly N elements
* in it.
*/
void registerSequence(std::vector<RPSChoice> recentHistory) {
	// Split the sequence into a key and value
	// value is the most recent player choice
	std::vector<RPSChoice> key(recentHistory.begin() + 1, recentHistory.end());
	RPSChoice value = recentHistory.back();

	// Add to the total, and to the count for the value
	data[key].counts[value] += 1;
	data[key].total += 1;
}

// Gets the next action most likely from the given one.
// We assume actions has N - 1 elements in it (i.e. the
// size of the window).
RPSChoice getMostLikely(std::vector<RPSChoice> recentHistory) {

	// Find the highest probability
	auto highestValue = 0;
	auto bestAction = RPSChoice::ROCK;

	// Get the list of previous actions that occurred
	// after the WINDOW_SIZE sequence
	std::vector<RPSChoice> actions;
	for (auto it : data[recentHistory].counts) {
		actions.push_back(it.first);
	}

	// Go through each action and determine the most likely
	// action
	for (auto action : actions) {
		if (data[recentHistory].counts[action] > highestValue) {
			highestValue = data[recentHistory].counts[action];
			bestAction = action;
		}
	}

	return bestAction;
}

int main() {
	bool keepRunning = true;
	std::string line;
	yourWins = 0;
	aiWins = 0;
	totalGames = 0;

	std::vector<RPSChoice> actions;

	while (keepRunning) {
		try {
			printOptions();
			std::getline(std::cin, line);
			actions.push_back(getOption(line));

			RPSChoice aiChoice = randomChoice();
			if (actions.size() > WINDOW_SIZE) {
				// Take only the last N actions
				std::vector<RPSChoice> recentHistory(actions.begin() + (actions.size() - N), actions.begin() + (actions.size()));
				registerSequence(recentHistory);
				// Remove the first element so that we have WINDOW_SIZE elements
				recentHistory.erase(recentHistory.begin());
				RPSChoice mostLikely = getMostLikely(recentHistory);
				aiChoice = winningOption(mostLikely);
			}

			std::cout << "Your choice: " << RPSChoiceToString(actions.back()) << std::endl;
			std::cout << "AI choice: " << RPSChoiceToString(aiChoice) << std::endl;

			int playerWon = rpsWinner(actions.back(), aiChoice);
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