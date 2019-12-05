/**
*
* This code is converted from the code found: https://www.gkbrk.com/wiki/hill_climbing/
* which was made by Gokberk Yaltirakli.
*
*/

#include <iostream>
#include <random>
#include <string>

// A string containing all possible characters
static const std::string PRINTABLE = "abcdefghijklmnopqrstuvwxyz";
static std::string available = PRINTABLE;
static std::string guessed = "";
static bool keepRunning = true;

/**
* Generates a random string of a given length made from
* characters in the PRINTABLE string.
*
* @param length The length of the desired string
* @return A randomly generated string made from all available
* characters on an English keyboard
*/
std::string generateRandomSolution(const size_t length = 13) {
	// The output sentence
	std::string output = "";
	// Loop through the length of the desired string
	for (size_t i = 0; i < length; i++) {
		// Append a random character to it
		output += PRINTABLE[rand() % PRINTABLE.length()];
	}
	return output;
}

/**
* This generates a distance metric between the two strings.
* This is calculated by the distance between each compared
* character in the two strings.
*
* @param target The final solution
* @param solution The solution we are evaluating
* @return A distance metric between the two strings
*/
int evaluate(std::string target, std::string solution) {
	// The distance metric between the two strings
	int diff = 0;
	// Loop through the targeted string's length
	for (size_t i = 0; i < target.length(); i++) {
		// The solution character at the index
		char s = solution[i];
		// The target character at the index
		char t = target[i];
		// Increase the distance metric by the absolute value
		// of the distance between the characters
		diff += abs(int(s) - int(t));
	}
	return diff;
}

/**
* This changes a random index into a random character
* available from the PRINTABLE string.
*
* @param solution A reference to the string we are modifying
*/
void mutateSolution(std::string& solution) {
	// Get a random index in the string
	size_t index = rand() % solution.length();
	// Change the character at the index to a random character
	solution[index] = PRINTABLE[rand() % PRINTABLE.length()];
}

void printOptions() {
	std::cout << "Available characters: " << available << std::endl;
	std::cout << "You guessed: " << guessed << std::endl;
}

bool isGuessed(char& c) {
	return (guessed.find(c) != std::string::npos);
}

bool printHiddenCharacters(std::string target) {
	bool guessed = true;

	for (auto& t : target) {
		if (isGuessed(t)) {
			std::cout << t;
		}
		else {
			std::cout << "_";
			guessed = false;
		}
		std::cout << " ";
	}
	std::cout << std::endl;
	
	return guessed;
}

int main() {
	std::cout << "Hangman" << std::endl;
	std::cout << "Type a desired sentence length: ";
	std::string line;
	std::string target;
	std::getline(std::cin, line);
	try {
		int length = std::stoi(line);
		// Generate the target sentence
		target = generateRandomSolution(length);
	}
	catch (std::invalid_argument const& ex) {
		std::cerr << ex.what() << std::endl;
		return -1;
	}
	catch (std::out_of_range const& ex) {
		std::cerr << ex.what() << std::endl;
		return -1;
	}

	std::string aiSolution = generateRandomSolution(target.length());
	int bestScore = evaluate(target, aiSolution);
	int iterations = 1;

	while (keepRunning) {
		if (bestScore == 0) {
			std::cout << "AI wins after " << iterations << " iterations!" << std::endl;
			keepRunning = false;
			break;
		}

		if (printHiddenCharacters(target)) {
			std::cout << "You win!" << std::endl;
			keepRunning = false;
			break;
		}
		printOptions();

		try {
			char c;
			int numFound = scanf_s(" %c", &c);
			char throwaway = getchar();
			if (isGuessed(c)) {
				std::cout << "You've already guessed this character." << std::endl;
			} else {
				guessed += c;
				available.erase(std::remove(available.begin(), available.end(), c), available.end());
			}

			std::string newSolution = aiSolution;
			mutateSolution(newSolution);
			int score = evaluate(target, newSolution);
			if (score < bestScore) {
				aiSolution = newSolution;
				bestScore = score;
			}
		}
		catch (std::exception & ex) {
			std::cerr << ex.what() << std::endl;
			keepRunning = false;
			break;
		}
		iterations++;
	}

	return 0;
}