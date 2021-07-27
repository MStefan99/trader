//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_NEURALNETWORK_H
#define TRADER_NEURALNETWORK_H

#include <vector>
#include <list>
#include <cmath>
#include <thread>
#include <iostream>

#include "input_validation.h"
#include "Matrix.h"


typedef std::vector<float> Column;
typedef std::vector<std::vector<float>> Rows;


class NeuralNetwork {
public:
	NeuralNetwork() = default;
	explicit NeuralNetwork(std::vector<size_t> topology);

	std::vector<size_t> getTopology() const;

	NeuralNetwork(const NeuralNetwork& network) = default;
	NeuralNetwork& operator=(const NeuralNetwork& network) = default;

	Column feedforward(const Column& input) const;

	void propagateBackwards(const Column& input, const Column& target, float eta = 0.0005);

	void train(const Rows& inputs, const Rows& outputs, float eta = 0.0005, size_t epochs = 1);

	void batchTrain(const Rows& inputs, const Rows& outputs,
			size_t start, size_t end, float eta = 0.0005, size_t epochs = 1);

	void fastTrain(const Rows& inputs, const Rows& outputs,
			float eta = 0.0005, size_t epochs = 1, size_t thread_num = std::thread::hardware_concurrency());

	static Column errorVector(const Column& actual, const Column& expected);
	static float error(const Column& actual, const Column& expected);

	static Matrix errorVector(const Matrix& actual, const Matrix& expected);
	static float error(const Matrix& actual, const Matrix& expected);

	friend std::ostream& operator<<(std::ostream& out, const NeuralNetwork& network);
	friend std::istream& operator>>(std::istream& in, NeuralNetwork& network);

protected:
	std::vector<Matrix> _weights {};
	std::vector<Matrix> _biases {};
	std::vector<size_t> _topology {};
};

#endif //TRADER_NEURALNETWORK_H
