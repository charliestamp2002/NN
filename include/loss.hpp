#ifndef LOSS_H
#define LOSS_H

#include "matrix.hpp"
#include <vector>
#include <functional>   // <- added
#include <cstddef>      // <- added for std::size_t

double mse(const Matrix& predicted, const Matrix& actual);
Matrix mse_derivative(const Matrix& predicted, const Matrix& actual);

double binary_cross_entropy(const Matrix& predicted, const Matrix& actual);
Matrix bce_derivative(const Matrix& predicted, const Matrix& actual);



#endif

