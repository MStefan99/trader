//
// Created by MStefan99 on 23.7.21.
//

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(std::vector<size_t> topology):
		_topology {topology} {
	for (auto it {topology.begin()}; it != topology.end(); ++it) {
		std::vector<float> biases {};
		biases.resize(*it);
		_biases.emplace_back(biases);
	}

	for (auto it {++topology.begin()}; it != topology.end(); ++it) {
		_weights.emplace_back(*(it - 1), *it);
	}
}


std::vector<float> NeuralNetwork::feedforward(const std::vector<float>& input) const {
	Matrix activation {input};

	activation = activation + _biases[0];
	for (size_t i {1}; i < _topology.size(); ++i) {
		activation = _weights[i - 1] * activation;
		activation = activation + _biases[i];
	}
	return static_cast<std::vector<float>>(activation);
}


void NeuralNetwork::propagateBackwards(const std::vector<float>& input,
		const std::vector<float>& expected, float eta) {
	std::vector<Matrix> activations {};
	Matrix activation {input};

	activation = activation + _biases[0];
	activations.push_back(activation);
	for (size_t i {1}; i < _topology.size(); ++i) {
		activation = _weights[i - 1] * activation;
		activation = activation + _biases[i];
		activations.push_back(activation);
	}

	auto error {errorVector(Matrix {expected}, activations.back())};
	_biases[_topology.size() - 1] = error * eta;

	for (size_t i {2}; i <= _topology.size(); ++i) {
		auto prevError {error};
		error = _weights[_topology.size() - i].transpose() * error;

		_biases[_topology.size() - i] += error * eta;
		_weights[_topology.size() - i] += activations[_topology.size() - i].transpose() * prevError * eta;
	}
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


Matrix NeuralNetwork::errorVector(const Matrix& actual, const Matrix& expected) {
	if (actual.getHeight() != expected.getHeight()) {
		throw std::length_error("Vector length mismatch");
	}

	Matrix result {actual.getHeight(), 1};
	for (size_t i {0}; i < actual.getHeight(); ++i) {
		result[i][0] = actual[i][0] - expected[i][0];
	}
	return result;
}


float NeuralNetwork::error(const Matrix& actual, const Matrix& expected) {
	if (actual.getHeight() != expected.getHeight()) {
		throw std::length_error("Vector length mismatch");
	}

	float err {0};
	for (size_t i {0}; i < actual.getHeight(); ++i) {
		err += static_cast<float>(std::pow(actual[i][0] - expected[i][0], 2));
	}
	return err;
}
