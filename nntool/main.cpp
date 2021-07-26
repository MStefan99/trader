#include <iostream>
#include <vector>
#include <random>

#include "NeuralNetwork.h"


float getResult(float a, float b) {
	return (a - 7) / 2 + 3 * (b + 11) - 13;
}


int main() {
	NeuralNetwork nn {{2, 1}};
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution {-10, 10};

	std::vector<std::vector<float>> inputs {};
	std::vector<std::vector<float>> outputs {};
	for (size_t i {0}; i < 10000000; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};

		inputs.push_back({a, b});
		outputs.push_back({getResult(a, b)});
	}

	std::cout << "Data ready!" << std::endl;
	nn.fastTrain(inputs, outputs, 0.01);

	size_t attempts {100};
	float errorSum {};
	for (size_t i {0}; i < attempts; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};

		std::vector<float> input {a, b};
		std::vector<float> output {getResult(a, b)};

		errorSum += NeuralNetwork::error(Matrix {nn.feedforward(input)}, Matrix {output});
	}

	std::cout << "Average error: " << errorSum / static_cast<float>(attempts) << std::endl;
	return 0;
}
