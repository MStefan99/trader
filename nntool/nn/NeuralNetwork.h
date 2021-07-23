//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_NEURALNETWORK_H
#define TRADER_NEURALNETWORK_H

#include <vector>

#include "Matrix.h"


class NeuralNetwork {
public:
	explicit NeuralNetwork(std::vector<unsigned int> topology, float learningRate = 0.0005);

	std::vector<float> feedforward(const std::vector<float>& input) const;

protected:
	std::vector<Matrix> _weights;
	std::vector<std::vector<float>> _biases;
	std::vector<unsigned int> _topology;
	float _learningRate;
};

#endif //TRADER_NEURALNETWORK_H
