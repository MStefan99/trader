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
	if (_topology.empty()) {
		throw std::runtime_error("Empty topology");
	}

	for (const auto& layer: _topology) {
		std::vector<float> biases {};
		biases.resize(layer);
		_biases.emplace_back(biases);
	}

	for (auto it {++topology.begin()}; it != topology.end(); ++it) {
		_weights.emplace_back(*(it - 1), *it);
		_weights.back().randomize();
	}
}


std::vector<size_t> NeuralNetwork::getTopology() const {
	return _topology;
}


std::vector<float> NeuralNetwork::feedforward(const std::vector<float>& input) const {
	if (_topology.empty()) {
		throw std::runtime_error("Empty topology");
	}
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
	if (_topology.empty()) {
		throw std::runtime_error("Empty topology");
	}
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
	_biases[_biases.size() - 1] += error * eta;
	_weights[_weights.size() - 1] += error * activations[activations.size() - 2].transpose() * eta;

	for (size_t i {2}; i < _topology.size(); ++i) {
		error = _weights[_weights.size() - i + 1].transpose() * error;

		_biases[_biases.size() - i] += error * eta;
		_weights[_weights.size() - i] += error * activations[activations.size() - i - 1].transpose() * eta;
	}
}


void NeuralNetwork::train(const Rows& inputs, const Rows& outputs,
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


void NeuralNetwork::batchTrain(const Rows& inputs, const Rows& outputs,
		size_t start, size_t end,
		float eta, size_t epochs) {
	if (inputs.size() != outputs.size()) {
		throw std::length_error("Input size does not equal output size");
	}
	if (inputs.size() < end) {
		end = inputs.size();
	}

	for (size_t e {0}; e < epochs; ++e) {
		for (size_t i {start}; i < end; ++i) {
			propagateBackwards(inputs[i], outputs[i], eta);
		}
	}
}


void NeuralNetwork::fastTrain(const Rows& inputs, const Rows& outputs,
		float eta, size_t epochs, size_t thread_num) {
	std::list<NeuralNetwork> networks {};
	std::list<std::thread> threads {};
	std::list<Matrix> weights {};
	std::list<Matrix> biases {};
	size_t batchSize {inputs.size() / thread_num};

	for (size_t i {0}; i < thread_num; ++i) {
		networks.emplace_back(_topology);

		threads.emplace_back(&NeuralNetwork::batchTrain, std::ref(networks.back()),
				std::ref(inputs), std::ref(outputs),
				i * batchSize, (i + 1) * batchSize,
				eta, epochs);
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


Column NeuralNetwork::errorVector(const Column& actual, const Column& expected) {
	if (actual.size() != expected.size()) {
		throw std::length_error("Vector length mismatch");
	}

	Column result {};
	for (size_t i {0}; i < actual.size(); ++i) {
		result.push_back({actual[i] - expected[i]});
	}
	return result;
}


float NeuralNetwork::error(const Column& actual, const Column& expected) {
	if (actual.size() != expected.size()) {
		throw std::length_error("Vector length mismatch");
	}

	float err {0};
	for (size_t i {0}; i < actual.size(); ++i) {
		err += static_cast<float>(std::abs(actual[i] - expected[i]));
	}
	return err;
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
		err += static_cast<float>(std::abs(actual[i][0] - expected[i][0]));
	}
	return err;
}


std::ostream& operator<<(std::ostream& out, const NeuralNetwork& network) {
	out << network._topology.size() << std::endl;
	for (size_t i {0}; i < network._topology.size(); ++i) {
		if (i) {
			out << ',';
		}
		out << network._topology[i];
	}
	out << std::endl;

	for (const auto& weightLayer: network._weights) {
		out << weightLayer;
	}
	for (const auto& biasLayer: network._biases) {
		out << biasLayer;
	}

	return out;
}


std::istream& operator>>(std::istream& in, NeuralNetwork& network) {
	size_t topologySize {};

	in >> topologySize;
	network._topology.resize(topologySize);
	network._weights.resize(topologySize - 1);
	network._biases.resize(topologySize);

	for (size_t i {}; i < topologySize; ++i) {
		if (i) {
			in >> ',';
		}
		in >> network._topology[i];
	}

	for (size_t i {}; i < topologySize - 1; ++i) {
		in >> network._weights[i];
	}

	for (size_t i {}; i < topologySize; ++i) {
		in >> network._biases[i];
	}

	return in;
}
