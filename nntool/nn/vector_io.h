//
// Created by MStefan99 on 26.7.21.
//

#ifndef TRADER_VECTOR_IO_H
#define TRADER_VECTOR_IO_H

#include <iostream>
#include <vector>

#include "Matrix.h"


std::ostream& operator<<(std::ostream& out, const std::vector<float>& vector);
std::istream& operator>>(std::istream& in, std::vector<float>& vector);

std::ostream& operator<<(std::ostream& out, const std::vector<std::vector<float>>& vector);
std::istream& operator>>(std::istream& in, std::vector<std::vector<float>>& vector);

#endif //TRADER_VECTOR_IO_H
