#include <iostream>

#include "NeuralNetwork.h"


int main() {
	NeuralNetwork nn {{2, 2, 2}};

	auto res = nn.feedforward({1, 2});
	std::cout << NeuralNetwork::error({1, 2}, res) << std::endl;
	return 0;
}
