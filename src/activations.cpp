#include "activations.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>

namespace nn {

Matrix softmax(const Matrix& m) {

    Matrix result(m.getRows(), m.getCols());

    for (size_t i = 0; i < m.getRows(); i++) {
        // Find max val for row i to stabilise softmax
        double max_val{0.0};

        for (size_t j = 0; j < m.getCols(); j++)
        {
            if (m(i, j) > max_val) {
                max_val = m(i, j);
            }
        }

        double sum{0.0};
        // Compute exp(j - max_val) and sum
        for (size_t j = 0; j < m.getCols(); j++) {
            result(i, j) = std::exp(m(i, j) - max_val);
            sum += result(i, j);
        }

        for (size_t j = 0; j < m.getCols(); j++) {
            result(i, j) = result(i, j) /= sum;
        }

    }

    return result;
}

}