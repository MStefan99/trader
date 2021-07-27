//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <stdexcept>
#include <random>
#include <iostream>

#include "input_validation.h"


class Matrix {
public:
	explicit Matrix(size_t w = 1, size_t h = 1);
	explicit Matrix(const std::vector<float>& vector);
	explicit Matrix(const std::vector<std::vector<float>>& vector);
	Matrix(const std::initializer_list<std::initializer_list<float>>& list);

	void randomize();

	std::vector<float>& operator[](size_t i);
	const std::vector<float>& operator[](size_t i) const;

	Matrix transpose() const;

	Matrix operator*(float scalar) const;
	Matrix operator/(float scalar) const;

	Matrix operator*(const Matrix& matrix) const;
	Matrix operator*(const std::vector<float>& vector) const;

	Matrix& operator*=(float scalar);
	Matrix& operator*=(const Matrix& matrix);
	Matrix& operator*=(const std::vector<float>& vector);

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator+(const std::vector<float>& vector) const;

	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator+=(const std::vector<float>& vector);

	Matrix multiplyComponents(const Matrix& matrix) const;
	Matrix concat(const Matrix& matrix) const;

	explicit operator std::vector<float>() const;
	explicit operator std::vector<std::vector<float>>() const;

	size_t getWidth() const;
	size_t getHeight() const;

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
	friend std::istream& operator>>(std::istream& in, Matrix& matrix);

protected:
	std::vector<std::vector<float>> _values {};
	size_t _w {};
	size_t _h {};
};

#endif //TRADER_MATRIX_H