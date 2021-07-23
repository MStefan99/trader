#include "NeuralNetwork.h"


int main() {
	NeuralNetwork nn {{5, 10, 5}};

	nn.feedforward({1, 2, 3, 4, 5});
	return 0;
}
