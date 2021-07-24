//
// Created by MStefan99 on 23.7.21.
//

#ifndef TRADER_MATRIX_H
#define TRADER_MATRIX_H

#include <vector>
#include <stdexcept>
#include <random>


class Matrix {
public:
	explicit Matrix(size_t w = 1, size_t h = 1);
	explicit Matrix(const std::vector<float>& vector);
	Matrix(const std::initializer_list<std::initializer_list<float>>& list);

	std::vector<float>& operator[](size_t i);
	const std::vector<float>& operator[](size_t i) const;

	Matrix transpose() const;

	Matrix operator*(float scalar) const;
	Matrix operator/(float scalar) const;

	Matrix operator*(const Matrix& matrix) const;
	Matrix operator*(const std::vector<float>& vector) const;

	Matrix& operator*=(float scalar) ;
	Matrix& operator*=(const Matrix& matrix) ;
	Matrix& operator*=(const std::vector<float>& vector) ;

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator+(const std::vector<float>& vector) const;

	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator+=(const std::vector<float>& vector);

	Matrix multiplyComponents(const Matrix& matrix) const;
	Matrix concat(const Matrix& matrix) const;

	explicit operator std::vector<float>() const;

	size_t getWidth() const;
	size_t getHeight() const;

protected:
	std::vector<std::vector<float>> _values;
	size_t _w;
	size_t _h;
};

#endif //TRADER_MATRIX_H
