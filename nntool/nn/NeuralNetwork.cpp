//
// Created by MStefan99 on 23.7.21.
//

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(std::vector<size_t> topology):
		_topology {topology} {
	for (auto it {++topology.begin()}; it != topology.end(); ++it) {
		std::vector<float> biases {};
		biases.resize(*(it - 1));
		_biases.push_back(biases);
		_weights.emplace_back(*(it - 1), *it);
	}
}


std::vector<float> NeuralNetwork::feedforward(const std::vector<float>& input) const {
	Matrix activations {input};

	for (size_t i {0}; i < _topology.size(); ++i) {
		if (i > 0) {
			activations = _weights[i - 1] * activations;
		}
		if (i < _biases.size()) {
			activations = activations + _biases[i];
		}
	}
	return static_cast<std::vector<float>>(activations);
}


void NeuralNetwork::propagateBackwards(const std::vector<float>& input, const std::vector<float>& target,
		float learningRate) {
}


void NeuralNetwork::train(const std::vector<std::vector<float>>& inputs, const std::vector<std::vector<float>>& outputs,
		size_t epochs, float eta) {
	if (inputs.size() != outputs.size()) {
		throw std::length_error("Input size does not equal output size");
	}

	for (size_t i {0}; i < epochs; ++i) {
		propagateBackwards(inputs[i], outputs[i], eta);
	}
}


float NeuralNetwork::error(const std::vector<float>& output, const std::vector<float>& expected) {
	if (output.size() != expected.size()) {
		throw std::length_error("Vector length mismatch");
	}

	float err {0};

	for (size_t i {0}; i < output.size(); ++i) {
		err += static_cast<float>(std::pow(output[i] - expected[i], 2));
	}
	return err;
}
