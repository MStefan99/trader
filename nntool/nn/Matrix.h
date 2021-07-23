//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>


class Matrix {
public:
	explicit Matrix(unsigned int w = 1, unsigned int h = 1);

	std::vector<float>& operator[](unsigned int i);

	Matrix transpose() const;

	Matrix operator*(float scalar) const;
	Matrix operator*(const Matrix& matrix) const;
	Matrix operator+(const Matrix& matrix) const;
	Matrix multiplyComponents(const Matrix& matrix) const;

	Matrix concat(const Matrix& matrix) const;

protected:
	std::vector<std::vector<float>> _values;
	unsigned int _w;
	unsigned int _h;
};

#endif //TRADER_MATRIX_H
