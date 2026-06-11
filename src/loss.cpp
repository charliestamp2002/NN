#include "layer.hpp"
#include "activations.hpp"
#include "matrix.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>

double mse(const Matrix& predicted, const Matrix& actual) {

    double sum{0.0};
    for (size_t i = 0; i < predicted.getRows(); i++) {
        for (size_t j = 0; j < predicted.getCols(); j++) {
            double diff = predicted(i, j) - actual(i, j);
            sum += diff * diff;

        }
    }
    double loss = sum / (predicted.getRows() * predicted.getCols());
    return loss;
};

Matrix mse_derivative(const Matrix& predicted, const Matrix& actual) {

    Matrix result(predicted.getRows(), predicted.getCols());

    for (size_t i = 0; i < predicted.getRows(); i++) {
        for (size_t j = 0; j < predicted.getCols(); j++) {

            result(i, j) = predicted(i, j) - actual(i, j);
        }
    }

    return result;

}



