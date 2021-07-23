//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_NEURALNETWORK_H
#define TRADER_NEURALNETWORK_H

#include "Matrix.h"


class NeuralNetwork {
public:
	explicit NeuralNetwork(unsigned int* topology, float learningRate = 0.0005);

protected:
	float _learningRate;
};

#endif //TRADER_NEURALNETWORK_H
