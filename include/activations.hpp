#ifndef ACTIVATIONS_HPP
#define ACTIVATIONS_HPP

#include "matrix.hpp"
#include <cmath>


namespace nn {

  // ==================== SIGMOID ====================

  inline double sigmoid(double x) {
      return 1.0 / (1.0 + std::exp(-x));
  }

  inline double sigmoid_prime(double x) {
      double s = sigmoid(x);
      return s * (1.0 - s);
  }

  // ==================== RELU ====================

  inline double relu(double x) {
      return x > 0.0 ? x : 0.0;
  }

  inline double relu_prime(double x) {
      return x > 0.0 ? 1.0 : 0.0;
  }

  // ==================== TANH ====================

  inline double tanh_activation(double x) {
      return std::tanh(x);
  }

  inline double tanh_prime(double x) {
      double t = std::tanh(x);
      return 1.0 - t * t;
  }

  // ==================== SOFTMAX (operates on matrices) ====================

  // Softmax for output layer (multi-class classification)
  // Converts vector to probability distribution (sums to 1)
  Matrix softmax(const Matrix& x);

  }  // namespace nn
#endif