//
// Created by MStefan99 on 26.7.21.
//

#ifndef TRADER_INPUT_VALIDATION_H
#define TRADER_INPUT_VALIDATION_H

#include <cstring>
#include <iostream>
#include <stdexcept>


template <class Traits>
std::basic_istream<char, Traits>& operator>>(std::basic_istream<char, Traits>& in, const char& c) {
	char buffer;

	in.read(&buffer, 1);
	if (buffer != c) {
		throw std::runtime_error("Error while parsing the input");
	}

	return in;
}


template <class Traits, std::size_t N>
std::basic_istream<char, Traits>& operator>>(std::basic_istream<char, Traits>& in, const char (& s)[N]) {
	char buffer[N] {};

	in.read(buffer, N - 1);
	if (strncmp(buffer, s, N - 1)) {
		throw std::runtime_error("Error while parsing the input");
	}

	return in;
}


#endif //TRADER_INPUT_VALIDATION_H
