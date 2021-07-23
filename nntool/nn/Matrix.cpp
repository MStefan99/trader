//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(size_t w, size_t h):
		_w {w}, _h {h} {
	for (size_t i {0}; i < h; ++i) {
		std::vector<float> v {};
		v.resize(w);
		_values.push_back(v);
	}
}


Matrix::Matrix(const std::vector<float>& vector):
		_w {1}, _h {vector.size()} {
	for (size_t i {0}; i < vector.size(); ++i) {
		_values.push_back(std::vector<float> {vector[i]});
	}
}


std::vector<float>& Matrix::operator[](size_t i) {
	return _values[i];
}


Matrix Matrix::transpose() const {
	Matrix result {_h, _w};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[i][j] = _values[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator*(float scalar) const {
	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * scalar;
		}
	}
	return result;
}


Matrix Matrix::operator*(const Matrix& matrix) const {
	if (_w != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {matrix._w, _h};
	for (size_t j {0}; j < matrix._w; ++j) {
		for (size_t i {0}; i < _h; ++i) {
			for (size_t k {0}; k < _w; ++k) {
				result[i][j] += _values[i][k] * matrix._values[k][j];
			}
		}
	}
	return result;
}


Matrix Matrix::operator*(const std::vector<float>& vector) const {
	if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {vector.size(), 1};

	for (size_t j {0}; j < vector.size(); ++j) {
		result[0][j] = _values[0][j] * vector[j];
	}
	return result;
}


Matrix Matrix::operator+(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::operator+(const std::vector<float>& vector) const {
	if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {1, vector.size()};

	for (size_t j {0}; j < vector.size(); ++j) {
		result[j][0] = _values[j][0] + vector[j];
	}
	return result;
}


Matrix Matrix::multiplyComponents(const Matrix& matrix) const {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] * matrix._values[j][i];
		}
	}
	return result;
}


Matrix Matrix::concat(const Matrix& matrix) const {
	if (_h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {_w + matrix._w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i];
		}
		for (size_t i {0}; i < matrix._w; ++i) {
			result[j][i + _w] = matrix._values[j][i];
		}
	}
	return result;
}


Matrix::operator std::vector<float>() const {
	if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	std::vector<float> result {};

	for (const auto& v: _values) {
		result.push_back(v[0]);
	}
	return result;
}
