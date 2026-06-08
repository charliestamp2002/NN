#ifndef LAYER_H
#define LAYER_H

#include "matrix.hpp"
#include <vector>
#include <functional>   // <- added
#include <cstddef>      // <- added for std::size_t

namespace nn {

enum class ActivationType {
    NONE, 
    SIGMOID,
    TANH,
    RELU,
};

class DenseLayer {
    private:
        size_t input_size_;
        size_t output_size_;
        Matrix weights_;
        Matrix biases_;
        
        std::function<double(double)> activation_;
        std::function<double(double)> activation_derivative_;

        // Cached values for backpropagation (we'll use these in Phase 3)
      Matrix last_input_;     // Store input from forward pass
      Matrix last_z_;         // Pre-activation values
      Matrix last_a_;         // Post-activation output

    public:
        DenseLayer(size_t input_size, size_t output_size, ActivationType activation = ActivationType::NONE);

        Matrix forward(const Matrix& input);
        Matrix backward(const Matrix& output_gradient, double learning_rate);

        // GETTERS

        size_t input_size() const;
        size_t output_size() const;

        // first const: returning a matrix we are not modifying, 
        // second const: This function does not modify the object it belongs to
        const Matrix& weights() const;
        const Matrix& biases() const;

        // Get cached values (useful for debugging)
        const Matrix& last_input() const;
        const Matrix& last_z() const;
        const Matrix& last_a() const;

        // SETTERS

        void set_weights(const Matrix& weights);
        void set_biases(const Matrix& biases);

    private:
        // Initialize weights based on activation type
        // He initialization for ReLU: σ = sqrt(2/n)
        // Xavier initialization for Sigmoid/Tanh: σ = sqrt(2/(n+m))
        void initialize_weights(ActivationType activation);

        // Set activation function pointers based on type
        void set_activation_functions(ActivationType activation);


    
};

}      

#endif
