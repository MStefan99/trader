#include <iostream>
#include <vector>
#include <random>

#include "NeuralNetwork.h"


float getResult(float a, float b) {
	return (a - 2) / 3 + 7 * (b + 5) - 11;
}


int main() {
	NeuralNetwork nn {{2, 1}};
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution {-10, 10};

	size_t trainingSet {100000};
	Matrix inputs {nn.getTopology().front(), trainingSet};
	Matrix outputs {nn.getTopology().back(), trainingSet};
	for (size_t i {0}; i < trainingSet; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};
		float res {getResult(a, b)};

		inputs[i] = {a, b};
		outputs[i] = {res};
	}

	std::cout << "Data ready!" << std::endl;
	nn.fastTrain(inputs, outputs, 0.01);
	// The larger the input values, the lower eta should be to avoid overshooting during training

	size_t attempts {100};
	float errorSum {};
	for (size_t i {0}; i < attempts; ++i) {
		float a {distribution(generator)};
		float b {distribution(generator)};

		std::vector<float> input {a, b};
		std::vector<float> output {getResult(a, b)};

		errorSum += NeuralNetwork::error(nn.feedforward(input), output);
	}

	std::cout << "Average error: " << errorSum / static_cast<float>(attempts) << std::endl;
	return 0;
}
