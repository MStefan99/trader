//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(unsigned int w, unsigned int h):
		_w {w}, _h {h} {
	for (unsigned int i {0}; i < h; ++i) {
		std::vector<float> v {};
		v.resize(w);
		_values.push_back(v);
	}
}


std::vector<float>& Matrix::operator[](unsigned int i) {
	return _values[i];
}


Matrix Matrix::transpose() const {
	Matrix result {_h, _w};

	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < _w; ++i) {
			result[i][j] = _values[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator*(float scalar) const {
	Matrix result {_w, _h};

	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * scalar;
		}
	}
	return result;
}


Matrix Matrix::operator*(const Matrix& matrix) const {
	Matrix result {matrix._w, _h};
	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < matrix._w; ++i) {
			float sum {};

			for (unsigned int k {0}; k < matrix._w; ++k) {
				sum += _values[i][k] * matrix._values[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


Matrix Matrix::operator+(const Matrix& matrix) const {
	Matrix result {_w, _h};

	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::multiplyComponents(const Matrix& matrix) const {
	Matrix result {_w, _h};

	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::concat(const Matrix& matrix) const {
	Matrix result {_w + matrix._w, _h};

	for (unsigned int j {0}; j < _h; ++j) {
		for (unsigned int i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i];
		}
		for (unsigned int i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix._values[j][i];
		}
	}
	return result;
}
