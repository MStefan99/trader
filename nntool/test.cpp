//
// Created by mikha on 31.12.21.
//

#include <iostream>

#include "Matrix.h"


int main() {
	Matrix a {{0.071884, 0.953617, 0.830076}, {0.654195, 0.37923, 0.0118257}, {0.391306, 0.0823461, 0.305495}};

	std::cout << a.invert() << std::endl;

	return 0;
}