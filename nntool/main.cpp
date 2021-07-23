#include "NeuralNetwork.h"
#include "Matrix.h"
#include <vector>


int main() {
	NeuralNetwork nn {{2, 2, 2}};

	nn._biases[0][0] = 11;

	nn._weights[0]._values[0][1] = 2;
	nn._weights[1]._values[0][1] = 7;
	nn._weights[0]._values[1][1] = 13;
	nn._weights[1]._values[1][1] = 17;

	auto res = nn.feedforward({1, 2});
	return 0;
}
