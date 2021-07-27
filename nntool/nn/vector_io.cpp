//
// Created by MStefan99 on 27.7.21.
//

#include "vector_io.h"


std::ostream& operator<<(std::ostream& out, const std::vector<float>& vector) {
	Matrix m {vector};
	out << m;
	return out;
}


std::istream& operator>>(std::istream& in, std::vector<float>& vector) {
	Matrix m {};
	in >> m;

	vector = static_cast<std::vector<float>>(m);
	return in;
}


std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<float>>& vector) {
	Matrix m {vector};
	out << m;
	return out;
}


std::istream& operator>>(std::istream& in, std::vector<std::vector<float>>& vector) {
	Matrix m {};
	in >> m;

	vector = static_cast<std::vector<std::vector<float>>>(m);
	return in;
}
