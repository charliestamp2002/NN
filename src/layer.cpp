#include "layer.hpp"
#include "activations.hpp"
#include "matrix.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>
#include <random>

namespace nn {

DenseLayer::DenseLayer(size_t input_size, size_t output_size, ActivationType activation) 
    :input_size_(input_size),
    output_size_(output_size)
    {
        initialize_weights(activation);
        set_activation_functions(activation);
    }

    void DenseLayer::initialize_weights(ActivationType activation) {
        double stddev{};

        if (activation == ActivationType::RELU) {
          stddev = std::sqrt(2.0 / static_cast<double>(input_size_));
      } else if (activation == ActivationType::SIGMOID ||
                 activation == ActivationType::TANH) {
          stddev = std::sqrt(2.0 / static_cast<double>(input_size_ + output_size_));
      } else {
          stddev = 0.01;
      }

      weights_ = Matrix::randn(input_size_, output_size_, 0.0, stddev);
      biases_ = Matrix::zeros(1, output_size_);
  
    }

    void DenseLayer::set_activation_functions(ActivationType activation) {
        switch (activation) {
            case ActivationType::SIGMOID:
              activation_ = sigmoid;
              activation_derivative_ = sigmoid_prime;
              break;
            case ActivationType::RELU:
              activation_ = relu;
              activation_derivative_ = relu_prime;
              break;
            case ActivationType::TANH:
              activation_ = tanh_activation;
              activation_derivative_ = tanh_prime;
              break;
            case ActivationType::NONE:
                activation_ = [](double x) {return x;};
                activation_derivative_ = [](double x) {return 1.0;};
                break;
            default:
                throw std::invalid_argument("Unknown activation type");
        }

         
    }




}
