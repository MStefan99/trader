//
// Created by MStefan99 on 23.7.21.
//

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(std::vector<unsigned int> topology, float learningRate):
		_topology {topology},
		_learningRate {learningRate} {
	for (auto it {++topology.begin()}; it != topology.end(); ++it) {
		std::vector<float> biases {};
		biases.resize(*(it - 1));
		_biases.push_back(biases);
		_weights.emplace_back(*(it - 1), *it);
	}
}


std::vector<float> NeuralNetwork::feedforward(const std::vector<float>& input) const {
	return {};
}
