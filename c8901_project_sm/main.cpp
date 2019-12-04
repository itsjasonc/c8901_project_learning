#include <iostream>
#include <random>
#include <string>
#include <functional>
#include <exception>

enum class CHOICE {
	PLAY,
	IGNORE
};

class State {
public:
	std::string name;
	std::function<void()> lambda;

	State() {}
	State(std::string _name) : name(_name) {}

	void run();
};

void State::run() {
	lambda();
}

class Dog {
public:
	const int MIN_HAPPINESS = 1;
	const int MAX_HAPPINESS = 10;
	int happiness;
	State* currentState;

	Dog();

	void ignore();
	void play();
private:
};

Dog::Dog() {
	happiness = MAX_HAPPINESS / 2;
}

void Dog::ignore() {
	happiness--;
	if (happiness < 0)
		happiness = 0;
}

void Dog::play() {
	happiness++;
	if (happiness > MAX_HAPPINESS)
		happiness = MAX_HAPPINESS;
}

void printOptions() {
	std::cout << "Your options:" << std::endl;
	std::cout << "Ignore the dog - I/i" << std::endl;
	std::cout << "Play with dog - P/p" << std::endl;
	std::cout << "Quit - Q/q" << std::endl;
}

CHOICE getOption(std::string line) {
	std::cout << "Your input: " << line << std::endl;
	std::for_each(line.begin(), line.end(), [](char& c) {
		c = ::tolower(c);
		});
	if (line == "i") {
		return CHOICE::IGNORE;
	}
	else if (line == "p") {
		return CHOICE::PLAY;
	}
	else if (line == "q") {
		throw std::exception("Quit called.");
	}

	throw std::exception("Unhandled option.");
}

const int NUM_STATES = 3;
State states[NUM_STATES];

void setDogeState(Dog& dog) {
	if (dog.happiness == dog.MAX_HAPPINESS) {
		if (dog.currentState != &states[0]) {
			dog.currentState = &states[0];
		}
	}
	else if (dog.happiness == dog.MIN_HAPPINESS) {
		if (dog.currentState != &states[1]) {
			dog.currentState = &states[1];
		}
	}
	else {
		if (dog.currentState != &states[2]) {
			dog.currentState = &states[2];
		}
	}
}

int main() {
	Dog doge;
	states[0].name = "happy";
	states[0].lambda = [] {
		std::cout << "DOG IS HAPPY!";
	};
	states[1].name = "unhappy";
	states[1].lambda = [] {
		std::cout << "DOG IS UNHAPPY!";
	};
	states[2].name = "satisfied";
	states[2].lambda = [] {
		std::cout << "DOG IS SATISFIED!";
	};

	std::string line;
	bool keepRunning = true;
	doge.currentState = &states[2];

	setDogeState(doge);

	while (keepRunning) {
		try {
			printOptions();
			std::getline(std::cin, line);
			CHOICE option = getOption(line);
			switch (option) {
			case CHOICE::IGNORE:
				doge.ignore();
				break;
			case CHOICE::PLAY:
				doge.play();
				break;
			}

			std::cout << "Doge's happiness: " << doge.happiness << std::endl;

			setDogeState(doge);
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
