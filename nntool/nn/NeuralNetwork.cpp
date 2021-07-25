//
// Created by MStefan99 on 23.7.21.
//

#include <list>
#include "NeuralNetwork.h"


template <class T>
static T avg(std::list<T> values) {
	T sum {values.front().getWidth(), values.front().getHeight()};

	for (const auto& v: values) {
		sum += v;
	}
	return sum / values.size();
}


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

		_biases[_topology.size() - i] += error * (eta / static_cast<float>(error.getHeight()));
		_weights[_topology.size() - i] += prevError * activations[_topology.size() - i].transpose() *
				(eta / static_cast<float>(prevError.getHeight()));
	}
}


void NeuralNetwork::train(const std::vector<std::vector<float>>& inputs,
		const std::vector<std::vector<float>>& outputs,
		float eta, size_t epochs) {
	if (inputs.size() != outputs.size()) {
		throw std::length_error("Input size does not equal output size");
	}

	for (size_t e {0}; e < epochs; ++e) {
		for (size_t i {0}; i < inputs.size(); ++i) {
			propagateBackwards(inputs[i], outputs[i], eta);
		}
	}
}


void NeuralNetwork::fastTrain(const std::vector<std::vector<float>>& inputs,
		const std::vector<std::vector<float>>& outputs,
		float eta, size_t epochs, size_t thread_num) {
	std::list<NeuralNetwork> networks {};
	std::list<std::thread> threads {};
	std::list<Matrix> weights {};
	std::list<Matrix> biases {};
	size_t batchSize {inputs.size() / thread_num};

	for (size_t i {0}; i < thread_num; ++i) {
		networks.emplace_back(_topology);

		std::vector<std::vector<float>> input_batch {inputs.begin() + i * batchSize,
				inputs.begin() + (i + 1) * batchSize - 1};
		std::vector<std::vector<float>> output_batch {outputs.begin() + i * batchSize,
				outputs.begin() + (i + 1) * batchSize - 1};

		threads.emplace_back(&NeuralNetwork::train, &networks.back(), input_batch, output_batch, eta, epochs);
	}

	for (auto& t : threads) {
		t.join();
	}

	for (size_t i {0}; i < _topology.size() - 1; ++i) {
		weights.clear();

		for (const auto& n: networks) {
			weights.push_back(n._weights[i]);
		}
		_weights[i] = avg(weights);
	}

	for (size_t i {0}; i < _topology.size(); ++i) {
		biases.clear();

		for (const auto& n: networks) {
			biases.push_back(n._biases[i]);
		}
		_biases[i] = avg(biases);
	}
}


Matrix NeuralNetwork::errorVector(const Matrix& actual, const Matrix& expected) {
	if (actual.getHeight() != expected.getHeight()) {
		throw std::length_error("Vector length mismatch");
	}

	Matrix result {1, actual.getHeight()};
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
