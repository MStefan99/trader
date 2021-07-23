#include <iostream>

#include "Matrix.h"


int main() {
	Matrix m1 {2, 2};
	Matrix m2 {2, 2};

	m1[0][0] = 0;
	m1[0][1] = 0;
	m1[1][0] = 1;
	m1[1][1] = 0;

	m2[0][0] = 0;
	m2[0][1] = 1;
	m2[1][0] = 0;
	m2[1][1] = 0;

	Matrix m = m1.concat(m2);

	return 0;
}
