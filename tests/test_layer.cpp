#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "activations.hpp"
#include "matrix.hpp"
#include "layer.hpp"
#include <cmath>

using namespace nn;

// ==================== LAYER CONSTRUCTION ====================

  TEST_CASE("DenseLayer construction", "[layer][constructor]") {
      DenseLayer layer(3, 5, ActivationType::RELU);

      CHECK(layer.input_size() == 3);
      CHECK(layer.output_size() == 5);
      CHECK(layer.weights().getRows() == 3);
      CHECK(layer.weights().getCols() == 5);
      CHECK(layer.biases().getRows() == 1);
      CHECK(layer.biases().getCols() == 5);
  }

  TEST_CASE("DenseLayer biases initialized to zero", "[layer][constructor]") {
      DenseLayer layer(2, 3, ActivationType::SIGMOID);

      for (size_t j = 0; j < 3; j++) {
          CHECK(layer.biases()(0, j) == 0.0);
      }
  }

  TEST_CASE("DenseLayer weights initialized randomly", "[layer][constructor]") {
      DenseLayer layer(5, 5, ActivationType::RELU);

      // Check that not all weights are the same
      bool has_different = false;
      double first = layer.weights()(0, 0);

      for (size_t i = 0; i < 5; i++) {
          for (size_t j = 0; j < 5; j++) {
              if (layer.weights()(i, j) != first) {
                  has_different = true;
                  break;
              }
          }
          if (has_different) break;
      }

      CHECK(has_different);
  }

  // ==================== FORWARD PASS - DIMENSIONS ====================

  TEST_CASE("Forward pass single sample dimensions", "[layer][forward]") {
      DenseLayer layer(3, 5, ActivationType::RELU);

      Matrix input = {{1.0, 2.0, 3.0}};  // 1×3
      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 1);
      CHECK(output.getCols() == 5);
  }

  TEST_CASE("Forward pass batch dimensions", "[layer][forward]") {
      DenseLayer layer(2, 4, ActivationType::SIGMOID);

      Matrix input = {{1.0, 2.0},
                      {3.0, 4.0},
                      {5.0, 6.0}};  // 3×2

      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 3);  // Batch size preserved
      CHECK(output.getCols() == 4);  // Output size
  }

  TEST_CASE("Forward pass dimension mismatch throws", "[layer][forward]") {
      DenseLayer layer(3, 2, ActivationType::RELU);

      Matrix input = {{1.0, 2.0}};  // Wrong: 1×2, expected 1×3

      CHECK_THROWS_AS(layer.forward(input), std::invalid_argument);
  }

  // ==================== FORWARD PASS - ACTIVATION FUNCTIONS ====================

  TEST_CASE("Forward pass with ReLU activation", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::RELU);

      Matrix input = {{1.0, 2.0}};
      Matrix output = layer.forward(input);

      // ReLU outputs should be >= 0
      for (size_t j = 0; j < 3; j++) {
          CHECK(output(0, j) >= 0.0);
      }
  }

  TEST_CASE("Forward pass with Sigmoid activation", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::SIGMOID);

      Matrix input = {{1.0, 2.0}};
      Matrix output = layer.forward(input);

      // Sigmoid outputs should be in (0, 1)
      for (size_t j = 0; j < 3; j++) {
          CHECK(output(0, j) > 0.0);
          CHECK(output(0, j) < 1.0);
      }
  }

  TEST_CASE("Forward pass with Tanh activation", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::TANH);

      Matrix input = {{1.0, 2.0}};
      Matrix output = layer.forward(input);

      // Tanh outputs should be in (-1, 1)
      for (size_t j = 0; j < 3; j++) {
          CHECK(output(0, j) > -1.0);
          CHECK(output(0, j) < 1.0);
      }
  }

  TEST_CASE("Forward pass with linear activation (NONE)", "[layer][forward]") {
      DenseLayer layer(2, 2, ActivationType::NONE);

      // Set weights to identity and biases to zero for predictable output
      Matrix weights = {{1.0, 0.0},
                       {0.0, 1.0}};
      Matrix biases = {{0.0, 0.0}};
      layer.set_weights(weights);
      layer.set_biases(biases);

      Matrix input = {{3.0, 4.0}};
      Matrix output = layer.forward(input);

      // Linear: output = input (with identity weights, zero bias)
      CHECK_THAT(output(0, 0), Catch::Matchers::WithinRel(3.0, 0.001));
      CHECK_THAT(output(0, 1), Catch::Matchers::WithinRel(4.0, 0.001));
  }

  // ==================== FORWARD PASS - NO NaN/Inf ====================

  TEST_CASE("Forward pass produces no NaN values", "[layer][forward]") {
      DenseLayer layer(5, 10, ActivationType::RELU);

      Matrix input = Matrix::random(3, 5, -10.0, 10.0);
      Matrix output = layer.forward(input);

      for (size_t i = 0; i < output.getRows(); i++) {
          for (size_t j = 0; j < output.getCols(); j++) {
              CHECK_FALSE(std::isnan(output(i, j)));
          }
      }
  }

  TEST_CASE("Forward pass produces no Inf values", "[layer][forward]") {
      DenseLayer layer(5, 10, ActivationType::SIGMOID);

      Matrix input = Matrix::random(3, 5, -10.0, 10.0);
      Matrix output = layer.forward(input);

      for (size_t i = 0; i < output.getRows(); i++) {
          for (size_t j = 0; j < output.getCols(); j++) {
              CHECK_FALSE(std::isinf(output(i, j)));
          }
      }
  }

  // ==================== FORWARD PASS - CACHING ====================

  TEST_CASE("Forward pass caches input", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::RELU);

      Matrix input = {{1.0, 2.0},
                      {3.0, 4.0}};

      layer.forward(input);

      // Check that input was cached
      CHECK(layer.last_input().getRows() == 2);
      CHECK(layer.last_input().getCols() == 2);
      CHECK(layer.last_input()(0, 0) == 1.0);
      CHECK(layer.last_input()(1, 1) == 4.0);
  }

  TEST_CASE("Forward pass caches pre-activation values", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::RELU);

      Matrix input = {{1.0, 2.0}};
      layer.forward(input);

      // last_z_ should have shape (1 × 3)
      CHECK(layer.last_z().getRows() == 1);
      CHECK(layer.last_z().getCols() == 3);
  }

  TEST_CASE("Forward pass caches post-activation values", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::SIGMOID);

      Matrix input = {{1.0, 2.0}};
      Matrix output = layer.forward(input);

      // last_a_ should equal output
      CHECK(layer.last_a().getRows() == output.getRows());
      CHECK(layer.last_a().getCols() == output.getCols());

      for (size_t i = 0; i < output.getRows(); i++) {
          for (size_t j = 0; j < output.getCols(); j++) {
              CHECK(layer.last_a()(i, j) == output(i, j));
          }
      }
  }

  // ==================== FORWARD PASS - BATCH PROCESSING ====================

  TEST_CASE("Forward pass handles large batch", "[layer][forward]") {
      DenseLayer layer(10, 5, ActivationType::RELU);

      Matrix input = Matrix::random(100, 10);  // 100 samples
      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 100);
      CHECK(output.getCols() == 5);
  }

  TEST_CASE("Forward pass batch consistency", "[layer][forward]") {
      DenseLayer layer(3, 2, ActivationType::SIGMOID);

      // Process individually
      Matrix input1 = {{1.0, 2.0, 3.0}};
      Matrix input2 = {{4.0, 5.0, 6.0}};

      Matrix out1 = layer.forward(input1);
      Matrix out2 = layer.forward(input2);

      // Process as batch
      Matrix batch = {{1.0, 2.0, 3.0},
                      {4.0, 5.0, 6.0}};
      Matrix batch_out = layer.forward(batch);

      // Results should match
      CHECK_THAT(batch_out(0, 0), Catch::Matchers::WithinRel(out1(0, 0), 0.001));
      CHECK_THAT(batch_out(0, 1), Catch::Matchers::WithinRel(out1(0, 1), 0.001));
      CHECK_THAT(batch_out(1, 0), Catch::Matchers::WithinRel(out2(0, 0), 0.001));
      CHECK_THAT(batch_out(1, 1), Catch::Matchers::WithinRel(out2(0, 1), 0.001));
  }

  // ==================== FORWARD PASS - DIFFERENT INPUTS ====================

  TEST_CASE("Different inputs produce different outputs", "[layer][forward]") {
      DenseLayer layer(2, 3, ActivationType::RELU);

      Matrix input1 = {{0.0, 0.0}};
      Matrix input2 = {{1.0, 1.0}};

      Matrix out1 = layer.forward(input1);
      Matrix out2 = layer.forward(input2);

      // Outputs should differ (with very high probability)
      bool different = false;
      for (size_t j = 0; j < 3; j++) {
          if (out1(0, j) != out2(0, j)) {
              different = true;
              break;
          }
      }

      CHECK(different);
  }

  // ==================== FORWARD PASS - MATHEMATICAL CORRECTNESS ====================

  TEST_CASE("Forward pass mathematical correctness", "[layer][forward]") {
      // Create layer with known weights and biases
      DenseLayer layer(2, 2, ActivationType::NONE);  // Linear for easy verification

      Matrix weights = {{1.0, 2.0},
                       {3.0, 4.0}};
      Matrix biases = {{0.5, 0.5}};

      layer.set_weights(weights);
      layer.set_biases(biases);

      Matrix input = {{1.0, 2.0}};
      Matrix output = layer.forward(input);

      // Manual calculation:
      // z0 = (1.0*1.0 + 2.0*3.0) + 0.5 = (1.0 + 6.0) + 0.5 = 7.5
      // z1 = (1.0*2.0 + 2.0*4.0) + 0.5 = (2.0 + 8.0) + 0.5 = 10.5

      CHECK_THAT(output(0, 0), Catch::Matchers::WithinRel(7.5, 0.001));
      CHECK_THAT(output(0, 1), Catch::Matchers::WithinRel(10.5, 0.001));
  }

  TEST_CASE("Forward pass with zero input", "[layer][forward]") {
      DenseLayer layer(3, 2, ActivationType::NONE);

      Matrix weights = {{1.0, 2.0},
                       {3.0, 4.0},
                       {5.0, 6.0}};
      Matrix biases = {{1.0, 2.0}};

      layer.set_weights(weights);
      layer.set_biases(biases);

      Matrix input = {{0.0, 0.0, 0.0}};
      Matrix output = layer.forward(input);

      // With zero input, output should equal biases
      CHECK_THAT(output(0, 0), Catch::Matchers::WithinRel(1.0, 0.001));
      CHECK_THAT(output(0, 1), Catch::Matchers::WithinRel(2.0, 0.001));
  }

  TEST_CASE("Forward pass ReLU kills negative pre-activations", "[layer][forward]") {
      DenseLayer layer(2, 2, ActivationType::RELU);

      // Set weights to produce negative pre-activation
      Matrix weights = {{-1.0, 1.0},
                       {-1.0, 1.0}};
      Matrix biases = {{0.0, 0.0}};

      layer.set_weights(weights);
      layer.set_biases(biases);

      Matrix input = {{1.0, 1.0}};
      Matrix output = layer.forward(input);

      // z0 = 1*(-1) + 1*(-1) = -2 → ReLU → 0
      // z1 = 1*(1) + 1*(1) = 2 → ReLU → 2

      CHECK(output(0, 0) == 0.0);
      CHECK(output(0, 1) == 2.0);
  }

  // ==================== EDGE CASES ====================

  TEST_CASE("Forward pass single input single output", "[layer][forward]") {
      DenseLayer layer(1, 1, ActivationType::SIGMOID);

      Matrix input = {{5.0}};
      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 1);
      CHECK(output.getCols() == 1);
      CHECK(output(0, 0) > 0.0);
      CHECK(output(0, 0) < 1.0);
  }

  TEST_CASE("Forward pass many inputs to one output", "[layer][forward]") {
      DenseLayer layer(100, 1, ActivationType::SIGMOID);

      Matrix input = Matrix::random(1, 100);
      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 1);
      CHECK(output.getCols() == 1);
  }

  TEST_CASE("Forward pass one input to many outputs", "[layer][forward]") {
      DenseLayer layer(1, 100, ActivationType::RELU);

      Matrix input = {{5.0}};
      Matrix output = layer.forward(input);

      CHECK(output.getRows() == 1);
      CHECK(output.getCols() == 100);
  }

