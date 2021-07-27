//
// Created by MStefan99 on 23.7.21.
//

#include "Matrix.h"


Matrix::Matrix(size_t w, size_t h):
		_w {w}, _h {h} {
	_values.resize(_h);
	for (size_t j {0}; j < _h; ++j) {
		_values[j].resize(_w);
	}
}


void Matrix::randomize() {
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<float> distribution {-.1, .1};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] = distribution(generator);
		}
	}
}


Matrix::Matrix(const std::initializer_list<std::initializer_list<float>>& values):
		_w {values.begin()->size()}, _h {values.size()} {
	for (const auto& row : values) {
		std::vector<float> v {};
		for (auto value: row) {
			v.push_back(value);
		}
		_values.push_back(v);
	}
}


Matrix::Matrix(const std::vector<float>& vector):
		_w {1}, _h {vector.size()} {
	for (size_t i {0}; i < vector.size(); ++i) {
		_values.push_back(std::vector<float> {vector[i]});
	}
}


Matrix::Matrix(const std::vector<std::vector<float>>& vector):
		_w {vector.front().size()}, _h {vector.size()} {
	for (size_t i {0}; i < vector.size(); ++i) {
		_values.emplace_back(vector[i]);
	}
}


std::vector<float>& Matrix::operator[](size_t i) {
	return _values[i];
}


const std::vector<float>& Matrix::operator[](size_t i) const {
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


Matrix Matrix::operator/(float scalar) const {
	Matrix result {_w, _h};

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			result[j][i] = _values[j][i] / scalar;
		}
	}
	return result;
}


Matrix Matrix::operator*(const Matrix& matrix) const {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*(matrix[0][0]);
	} else if (_w != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {matrix._w, _h};
	for (size_t j {0}; j < matrix._w; ++j) {
		for (size_t i {0}; i < _h; ++i) {
			float sum {0};
			for (size_t k {0}; k < _w; ++k) {
				sum += _values[i][k] * matrix._values[k][j];
			}
			result[i][j] = sum;
		}
	}
	return result;
}


Matrix Matrix::operator*(const std::vector<float>& vector) const {
	if (vector.size() == 1) {
		return operator*(vector[0]);
	} else if (_w != 1 || _h != vector.size()) {
		throw std::length_error("Matrix dimension mismatch");
	}

	Matrix result {vector.size(), 1};

	for (size_t j {0}; j < vector.size(); ++j) {
		result[0][j] = _values[0][j] * vector[j];
	}
	return result;
}


Matrix& Matrix::operator*=(float scalar) {
	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] = _values[j][i] * scalar;
		}
	}
	return *this;
}


Matrix& Matrix::operator*=(const Matrix& matrix) {
	if (matrix._w == 1 && matrix._h == 1) {
		return operator*=(matrix[0][0]);
	} else if (_w != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	for (size_t j {0}; j < matrix._w; ++j) {
		for (size_t i {0}; i < _h; ++i) {
			for (size_t k {0}; k < _w; ++k) {
				_values[i][j] += _values[i][k] * matrix._values[k][j];
			}
		}
	}
	return *this;
}


Matrix& Matrix::operator*=(const std::vector<float>& vector) {
	if (vector.size() == 1) {
		return operator*=(vector[0]);
	} else if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	for (size_t j {0}; j < vector.size(); ++j) {
		_values[0][j] = _values[0][j] * vector[j];
	}
	return *this;
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


Matrix& Matrix::operator+=(const Matrix& matrix) {
	if (_w != matrix._w || _h != matrix._h) {
		throw std::length_error("Matrix dimension mismatch");
	}

	for (size_t j {0}; j < _h; ++j) {
		for (size_t i {0}; i < _w; ++i) {
			_values[j][i] = _values[j][i] + matrix._values[j][i];
		}
	}
	return *this;
}


Matrix& Matrix::operator+=(const std::vector<float>& vector) {
	if (_w != 1) {
		throw std::length_error("Matrix dimension mismatch");
	}

	for (size_t j {0}; j < vector.size(); ++j) {
		_values[j][0] = _values[j][0] + vector[j];
	}
	return *this;
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


Matrix::operator std::vector<std::vector<float>>() const {
	return _values;
}


size_t Matrix::getWidth() const {
	return _w;
}


size_t Matrix::getHeight() const {
	return _h;
}


std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
	out << matrix._w << ':' << matrix._h << std::endl;

	for (size_t j {0}; j < matrix._h; ++j) {
		for (size_t i {0}; i < matrix._w; ++i) {
			if (i) {
				out << ',';
			}
			out << matrix[j][i];
		}
		out << std::endl;
	}

	return out;
}


std::istream& operator>>(std::istream& in, Matrix& matrix) {
	in >> matrix._w >> ':' >> matrix._h;

	matrix._values.resize(matrix._h);

	for (size_t j {0}; j < matrix._h; ++j) {
		matrix._values[j].resize(matrix._w);

		for (size_t i {0}; i < matrix._w; ++i) {
			if (i) {
				in >> ',';
			}
			in >> matrix[j][i];
		}
	}

	return in;
}
