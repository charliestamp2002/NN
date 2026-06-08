#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "activations.hpp"
#include "matrix.hpp"
#include <cmath>

using namespace nn;

  TEST_CASE("Sigmoid at zero", "[activations][sigmoid]") {
      CHECK_THAT(sigmoid(0.0), Catch::Matchers::WithinRel(0.5, 0.001));
  }

  TEST_CASE("Sigmoid for large positive value", "[activations][sigmoid]") {
      // sigmoid(10) should be very close to 1
      CHECK_THAT(sigmoid(10.0), Catch::Matchers::WithinRel(0.9999, 0.001));
  }

  TEST_CASE("Sigmoid for large negative value", "[activations][sigmoid]") {
      // sigmoid(-10) should be very close to 0
      CHECK(sigmoid(-10.0) < 0.0001);
  }

  TEST_CASE("Sigmoid output range", "[activations][sigmoid]") {
      // Test various inputs, all should be in (0, 1)
      for (double x = -5.0; x <= 5.0; x += 0.5) {
          double s = sigmoid(x);
          CHECK(s > 0.0);
          CHECK(s < 1.0);
      }
  }

  TEST_CASE("Sigmoid is symmetric around 0.5", "[activations][sigmoid]") {
      // sigmoid(-x) = 1 - sigmoid(x)
      double x = 2.0;
      CHECK_THAT(sigmoid(-x), Catch::Matchers::WithinRel(1.0 - sigmoid(x), 0.001));
  }

  TEST_CASE("Sigmoid derivative at zero", "[activations][sigmoid]") {
      // σ'(0) = σ(0) * (1 - σ(0)) = 0.5 * 0.5 = 0.25
      CHECK_THAT(sigmoid_prime(0.0), Catch::Matchers::WithinRel(0.25, 0.001));
  }

  TEST_CASE("Sigmoid derivative is always positive", "[activations][sigmoid]") {
      for (double x = -5.0; x <= 5.0; x += 0.5) {
          CHECK(sigmoid_prime(x) > 0.0);
      }
  }

  TEST_CASE("Sigmoid derivative formula", "[activations][sigmoid]") {
      // σ'(x) = σ(x) * (1 - σ(x))
      double x = 1.5;
      double s = sigmoid(x);
      double expected = s * (1.0 - s);
      CHECK_THAT(sigmoid_prime(x), Catch::Matchers::WithinRel(expected, 0.001));
  }

  TEST_CASE("Sigmoid derivative at extremes is near zero", "[activations][sigmoid]") {
      // At large |x|, sigmoid saturates, derivative → 0
      CHECK(sigmoid_prime(10.0) < 0.001);
      CHECK(sigmoid_prime(-10.0) < 0.001);
  }

  TEST_CASE("Sigmoid with Matrix apply", "[activations][sigmoid]") {
      Matrix z = {{-2.0, -1.0, 0.0, 1.0, 2.0}};
      Matrix a = z.apply(sigmoid);

      CHECK_THAT(a(0, 0), Catch::Matchers::WithinRel(sigmoid(-2.0), 0.001));
      CHECK_THAT(a(0, 2), Catch::Matchers::WithinRel(0.5, 0.001));
      CHECK_THAT(a(0, 4), Catch::Matchers::WithinRel(sigmoid(2.0), 0.001));
  }

  // ==================== RELU ====================

  TEST_CASE("ReLU positive values pass through", "[activations][relu]") {
      CHECK(relu(1.0) == 1.0);
      CHECK(relu(5.0) == 5.0);
      CHECK(relu(0.5) == 0.5);
      CHECK(relu(100.0) == 100.0);
  }

  TEST_CASE("ReLU negative values become zero", "[activations][relu]") {
      CHECK(relu(-1.0) == 0.0);
      CHECK(relu(-5.0) == 0.0);
      CHECK(relu(-0.5) == 0.0);
      CHECK(relu(-100.0) == 0.0);
  }

  TEST_CASE("ReLU at zero", "[activations][relu]") {
      CHECK(relu(0.0) == 0.0);
  }

  TEST_CASE("ReLU derivative for positive values", "[activations][relu]") {
      CHECK(relu_prime(1.0) == 1.0);
      CHECK(relu_prime(5.0) == 1.0);
      CHECK(relu_prime(0.01) == 1.0);
      CHECK(relu_prime(100.0) == 1.0);
  }

  TEST_CASE("ReLU derivative for negative values", "[activations][relu]") {
      CHECK(relu_prime(-1.0) == 0.0);
      CHECK(relu_prime(-5.0) == 0.0);
      CHECK(relu_prime(-0.01) == 0.0);
      CHECK(relu_prime(-100.0) == 0.0);
  }

  TEST_CASE("ReLU derivative at zero", "[activations][relu]") {
      CHECK(relu_prime(0.0) == 0.0);
  }

  TEST_CASE("ReLU with Matrix apply", "[activations][relu]") {
      Matrix z = {{-2.0, -1.0, 0.0, 1.0, 2.0}};
      Matrix a = z.apply(relu);

      CHECK(a(0, 0) == 0.0);
      CHECK(a(0, 1) == 0.0);
      CHECK(a(0, 2) == 0.0);
      CHECK(a(0, 3) == 1.0);
      CHECK(a(0, 4) == 2.0);
  }

  TEST_CASE("ReLU produces sparse activations", "[activations][relu]") {
      Matrix z = {{-1, 2, -3, 4, -5}};
      Matrix a = z.apply(relu);

      // Count zeros
      int zero_count = 0;
      for (size_t j = 0; j < z.getCols(); j++) {
          if (a(0, j) == 0.0) zero_count++;
      }

      CHECK(zero_count == 3);  // 3 negative values become 0
  }

  // ==================== TANH ====================

  TEST_CASE("Tanh at zero", "[activations][tanh]") {
      CHECK(tanh_activation(0.0) == 0.0);
  }

  TEST_CASE("Tanh output range", "[activations][tanh]") {
      // tanh outputs in (-1, 1)
      for (double x = -5.0; x <= 5.0; x += 0.5) {
          double t = tanh_activation(x);
          CHECK(t > -1.0);
          CHECK(t < 1.0);
      }
  }

  TEST_CASE("Tanh for large positive value", "[activations][tanh]") {
      CHECK_THAT(tanh_activation(10.0), Catch::Matchers::WithinRel(1.0, 0.01));
  }

  TEST_CASE("Tanh for large negative value", "[activations][tanh]") {
      CHECK_THAT(tanh_activation(-10.0), Catch::Matchers::WithinRel(-1.0, 0.01));
  }

  TEST_CASE("Tanh is antisymmetric", "[activations][tanh]") {
      // tanh(-x) = -tanh(x)
      double x = 2.0;
      CHECK_THAT(tanh_activation(-x), Catch::Matchers::WithinRel(-tanh_activation(x), 0.001));
  }

  TEST_CASE("Tanh derivative at zero", "[activations][tanh]") {
      // tanh'(0) = 1 - tanh²(0) = 1 - 0 = 1
      CHECK_THAT(tanh_prime(0.0), Catch::Matchers::WithinRel(1.0, 0.001));
  }

  TEST_CASE("Tanh derivative formula", "[activations][tanh]") {
      // tanh'(x) = 1 - tanh²(x)
      double x = 1.5;
      double t = tanh_activation(x);
      double expected = 1.0 - t * t;
      CHECK_THAT(tanh_prime(x), Catch::Matchers::WithinRel(expected, 0.001));
  }

  TEST_CASE("Tanh derivative is always positive", "[activations][tanh]") {
      for (double x = -5.0; x <= 5.0; x += 0.5) {
          CHECK(tanh_prime(x) > 0.0);
      }
  }

  TEST_CASE("Tanh derivative at extremes is near zero", "[activations][tanh]") {
      // At large |x|, tanh saturates, derivative → 0
      CHECK(tanh_prime(10.0) < 0.01);
      CHECK(tanh_prime(-10.0) < 0.01);
  }

  TEST_CASE("Tanh with Matrix apply", "[activations][tanh]") {
      Matrix z = {{-2.0, -1.0, 0.0, 1.0, 2.0}};
      Matrix a = z.apply(tanh_activation);

      CHECK_THAT(a(0, 0), Catch::Matchers::WithinRel(std::tanh(-2.0), 0.001));
      CHECK(a(0, 2) == 0.0);
      CHECK_THAT(a(0, 4), Catch::Matchers::WithinRel(std::tanh(2.0), 0.001));
  }

  // ==================== SOFTMAX ====================

  TEST_CASE("Softmax single row sums to 1", "[activations][softmax]") {
      Matrix x = {{1.0, 2.0, 3.0}};
      Matrix result = softmax(x);

      double sum = 0.0;
      for (size_t j = 0; j < result.getCols(); j++) {
          sum += result(0, j);
      }

      CHECK_THAT(sum, Catch::Matchers::WithinRel(1.0, 0.001));
  }

  TEST_CASE("Softmax all outputs are positive", "[activations][softmax]") {
      Matrix x = {{-5.0, 0.0, 5.0}};
      Matrix result = softmax(x);

      for (size_t j = 0; j < result.getCols(); j++) {
          CHECK(result(0, j) > 0.0);
          CHECK(result(0, j) < 1.0);
      }
  }

  TEST_CASE("Softmax largest input gets largest probability", "[activations][softmax]") {
      Matrix x = {{1.0, 3.0, 2.0}};
      Matrix result = softmax(x);

      // Index 1 has largest input (3.0), should have largest probability
      CHECK(result(0, 1) > result(0, 0));
      CHECK(result(0, 1) > result(0, 2));
  }

  TEST_CASE("Softmax with uniform inputs", "[activations][softmax]") {
      Matrix x = {{1.0, 1.0, 1.0}};
      Matrix result = softmax(x);

      // All equal inputs → all equal probabilities (1/3)
      CHECK_THAT(result(0, 0), Catch::Matchers::WithinRel(0.333, 0.01));
      CHECK_THAT(result(0, 1), Catch::Matchers::WithinRel(0.333, 0.01));
      CHECK_THAT(result(0, 2), Catch::Matchers::WithinRel(0.333, 0.01));
  }

  TEST_CASE("Softmax batch processing", "[activations][softmax]") {
      Matrix x = {{1.0, 2.0, 3.0},
                  {3.0, 2.0, 1.0}};

      Matrix result = softmax(x);

      REQUIRE(result.getRows() == 2);
      REQUIRE(result.getCols() == 3);

      // Each row should sum to 1
      double sum_row0 = 0.0, sum_row1 = 0.0;
      for (size_t j = 0; j < 3; j++) {
          sum_row0 += result(0, j);
          sum_row1 += result(1, j);
      }

      CHECK_THAT(sum_row0, Catch::Matchers::WithinRel(1.0, 0.001));
      CHECK_THAT(sum_row1, Catch::Matchers::WithinRel(1.0, 0.001));
  }

  TEST_CASE("Softmax with large values (numerical stability)", "[activations][softmax]") {
      Matrix x = {{1000.0, 1001.0, 1002.0}};
      Matrix result = softmax(x);

      // Should not produce NaN or Inf
      for (size_t j = 0; j < result.getCols(); j++) {
          CHECK_FALSE(std::isnan(result(0, j)));
          CHECK_FALSE(std::isinf(result(0, j)));
      }

      // Should still sum to 1
      double sum = 0.0;
      for (size_t j = 0; j < result.getCols(); j++) {
          sum += result(0, j);
      }
      CHECK_THAT(sum, Catch::Matchers::WithinRel(1.0, 0.001));
  }

  TEST_CASE("Softmax with negative values", "[activations][softmax]") {
      Matrix x = {{-1.0, -2.0, -3.0}};
      Matrix result = softmax(x);

      // All should be positive
      for (size_t j = 0; j < result.getCols(); j++) {
          CHECK(result(0, j) > 0.0);
      }

      // Should sum to 1
      double sum = result(0, 0) + result(0, 1) + result(0, 2);
      CHECK_THAT(sum, Catch::Matchers::WithinRel(1.0, 0.001));

      // Largest input (-1.0) should have largest probability
      CHECK(result(0, 0) > result(0, 1));
      CHECK(result(0, 1) > result(0, 2));
  }

  TEST_CASE("Softmax single element", "[activations][softmax]") {
      Matrix x = {{5.0}};
      Matrix result = softmax(x);

      // Only one element, probability must be 1.0
      CHECK_THAT(result(0, 0), Catch::Matchers::WithinRel(1.0, 0.001));
  }

  // ==================== COMBINED USAGE ====================

  TEST_CASE("Activation functions compose with matrix operations", "[activations]") {
      Matrix z = {{-1.0, 0.0, 1.0}};

      // Apply sigmoid
      Matrix a1 = z.apply(sigmoid);

      // Apply ReLU  
      Matrix a2 = z.apply(relu);

      // Results should be different
      CHECK(a1(0, 0) != a2(0, 0));
  }

  TEST_CASE("Derivative chain for backprop", "[activations]") {
      // Simulating: gradient * activation_derivative
      Matrix z = {{1.0, 2.0}};
      Matrix gradient = {{0.5, 0.3}};

      Matrix sigmoid_grad = z.apply(sigmoid_prime);
      Matrix result = gradient.hadamard(sigmoid_grad);

      // Each element should be gradient * derivative
      double expected0 = 0.5 * sigmoid_prime(1.0);
      double expected1 = 0.3 * sigmoid_prime(2.0);

      CHECK_THAT(result(0, 0), Catch::Matchers::WithinRel(expected0, 0.001));
      CHECK_THAT(result(0, 1), Catch::Matchers::WithinRel(expected1, 0.001));
  }
