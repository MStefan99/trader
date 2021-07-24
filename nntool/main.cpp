#include <iostream>
#include <vector>
#include <cstdlib>

#include "NeuralNetwork.h"


float getResult(float a, float b) {
	return (a - 7) / 2 + 3 * (b + 11) - 13;
}


int main() {
	NeuralNetwork nn {{2, 1}};
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution {-10, 10};

	for (size_t i {0}; i < 100000; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};

		std::vector<float> input {a, b};
		std::vector<float> output {getResult(a, b)};

		nn.propagateBackwards(input, output, 0.01);
	}

	for (size_t i {0}; i < 20; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};

		std::vector<float> input {a, b};
		std::vector<float> output {getResult(a, b)};

		std::cout << "f(" << a << ", " << b << ") = " << getResult(a, b)
		          << " (predicted: " << nn.feedforward(input)[0] << ", loss: "
		          << NeuralNetwork::error(Matrix {nn.feedforward(input)}, Matrix {output}) << ")" << std::endl;
	}

	return 0;
}
