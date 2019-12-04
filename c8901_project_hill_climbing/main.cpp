/**
* The hill climbing code is based on the pseudocode found in the textbook
* Artificial Intelligence for Games (2nd edition) by Ian Millington.
*/
#include <iostream>
#include <vector>

enum class CHOICE {
	FEED,
	PLAY,
	SLEEP
};

std::vector<CHOICE> parameters;

int getValue(CHOICE choice) {
	return 0;
}

std::vector<CHOICE> optimizeParameters(std::vector<CHOICE>&  parameters) {
	// holds the best parameter change so far
	int bestParameterIndex = -1;
	int bestTweak = 0;

	// The initial best value is the value of the current parameters,
	// no point changing to a worse set
	int bestValue = getValue(parameters);

	// Loop through each parameter
	for (int i = 0; i < parameters.size(); i++) {

	}

	return parameters;
}

int main() {
	return 0;
}