#include <iostream>
#include <vector>
#include <cstdlib>

#include "NeuralNetwork.h"


int main() {
	NeuralNetwork nn {{2, 1}};
	srand(time(nullptr));

	for (size_t i {0}; i < 1000; ++i) {
		float a {static_cast<float>(rand())};
		float b {static_cast<float>(rand())};

		std::vector<float> input {a, b};
		std::vector<float> output {a + b};

		nn.propagateBackwards(input, output);

		std::cout << NeuralNetwork::error(nn.feedforward(input), output) << std::endl;
	}
	return 0;
}
