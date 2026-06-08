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


Matrix DenseLayer::forward(const Matrix& input) {

    if (input.getCols() != input_size_) {
        throw std::invalid_argument("number of weights do not equal input size");
    }

    //  Linear Transformation: Z = X.W
    // input dim: batch_size x input_size
    // weights dim input_size x output_size
    // output dim (Z): batch_size x output_size

    Matrix z = input * weights_;
    // Z = X.W + b
    // bias dim: (1 x output_size) i.e. 1 row 
    // add bias: Broadcast bias to every 

    for (size_t i = 0; i < z.getRows(); i++) {
        for (size_t j = 0; j < z.getCols(); j++) {
            z(i, j) += biases_(0, j);
        }
    }

    // apply activation once biases added:
    Matrix a = z.apply(activation_);

    // cache outputs:
    last_input_ = input;
    last_z_ = z;
    last_a_ = a;

    return a;
}

size_t DenseLayer::input_size() const {
    return input_size_;
}

size_t DenseLayer::output_size() const {
    return output_size_;
}

const Matrix& DenseLayer::weights() const {
    return weights_;
}

const Matrix& DenseLayer::biases() const {
    return biases_;
}

const Matrix& DenseLayer::last_input() const {
    return last_input_;
}

const Matrix& DenseLayer::last_z() const {
    return last_z_;
}

const Matrix& DenseLayer::last_a() const {
    return last_a_;
}

void DenseLayer::set_weights(const Matrix& weights) {
    if (weights.getRows() != input_size_ || weights.getCols() != output_size_) {
        throw std::invalid_argument("Weight dimensions don't match layer dimensions");
    }
    weights_ = weights;
}

void DenseLayer::set_biases(const Matrix& biases) {
    if (biases.getRows() != 1 || biases.getCols() != output_size_) {
        throw std::invalid_argument("Bias dimensions don't match layer dimensions");
    }
    biases_ = biases;
}




}
