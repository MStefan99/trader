//
// Created by MStefan99 on 26.7.21.
//

#ifndef TRADER_VECTOR_IO_H
#define TRADER_VECTOR_IO_H

#include <iostream>
#include <vector>

#include "input_validation.h"
#include "Matrix.h"


template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
	Matrix m {vector};
	out << m;
	return out;
}


template <class T>
std::istream& operator>>(std::istream& in, std::vector<T>& vector) {
	Matrix m {};
	in >> m;

	vector = static_cast<std::vector<T>>(m);
	return in;
}


#endif //TRADER_VECTOR_IO_H
