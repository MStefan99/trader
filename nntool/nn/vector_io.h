//
// Created by MStefan99 on 26.7.21.
//

#ifndef TRADER_VECTOR_IO_H
#define TRADER_VECTOR_IO_H

#include <iostream>
#include <vector>

#include "input_validation.h"


template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
	out << vector.size() << std::endl;

	for (size_t i {0}; i < vector.size(); ++i) {
		if (i) {
			out << ',';
		}
		out << vector[i];
	}
	return out;
}


template <class T>
std::istream& operator>>(std::istream& in, std::vector<T>& vector) {
	size_t vectorSize {};

	in >> vectorSize;
	vector.resize(vectorSize);

	for (size_t i {}; i < vectorSize; ++i) {
		if (i) {
			in >> ',';
		}
		in >> vector[i];
	}
	return in;
}


#endif //TRADER_VECTOR_IO_H
