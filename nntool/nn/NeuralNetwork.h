//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_NEURALNETWORK_H
#define TRADER_NEURALNETWORK_H

#include <vector>
#include <list>
#include <cmath>
#include <thread>

#include "Matrix.h"


class NeuralNetwork {
public:
	explicit NeuralNetwork(std::vector<size_t> topology);

	NeuralNetwork(const NeuralNetwork& network) = default;
	NeuralNetwork& operator=(const NeuralNetwork& network) = default;

	std::vector<float> feedforward(const std::vector<float>& input) const;

	void propagateBackwards(const std::vector<float>& input,
			const std::vector<float>& target, float eta = 0.0005);

	void train(const std::vector<std::vector<float>>& inputs,
			const std::vector<std::vector<float>>& outputs,
			float eta = 0.0005, size_t epochs = 1);

	void fastTrain(const std::vector<std::vector<float>>& inputs,
			const std::vector<std::vector<float>>& outputs,
			float eta = 0.0005, size_t epochs = 1, size_t thread_num = std::thread::hardware_concurrency());

	static Matrix errorVector(const Matrix& actual,
			const Matrix& expected);

	static float error(const Matrix& actual,
			const Matrix& expected);

protected:
	std::vector<Matrix> _weights;
	std::vector<Matrix> _biases;
	std::vector<size_t> _topology;
};

#endif //TRADER_NEURALNETWORK_H
