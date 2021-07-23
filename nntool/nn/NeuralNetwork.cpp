//
// Created by MStefan99 on 23.7.21.
//

#include "NeuralNetwork.h"


NeuralNetwork::NeuralNetwork(unsigned int* topology, float learningRate):
		_learningRate {learningRate} {
	while (*topology) {
		++topology;
	}
}
