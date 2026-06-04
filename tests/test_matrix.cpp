#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"


TEST_CASE("Default Constructor creates empty matrix", "[Matrix][Constructor]") {
    Matrix m;
    CHECK(m.getRows() == 0);
    CHECK(m.getCols() == 0);
}

 TEST_CASE("Dimension constructor creates zero-initialized matrix", "[matrix][constructor]") {
      Matrix m(3, 4);

      REQUIRE(m.getRows() == 3);
      REQUIRE(m.getCols() == 4);

      // All elements should be 0
      for (size_t i = 0; i < 3; i++) {
          for (size_t j = 0; j < 4; j++) {
              CHECK(m(i, j) == 0.0);
          }
      }
  }

  TEST_CASE("Dimension constructor with single row", "[matrix][constructor]") {
      Matrix m(1, 5);
      CHECK(m.getRows() == 1);
      CHECK(m.getCols() == 5);
  }

  TEST_CASE("Dimension constructor with single column", "[matrix][constructor]") {
      Matrix m(5, 1);
      CHECK(m.getRows() == 5);
      CHECK(m.getCols() == 1);
  }

  TEST_CASE("Fill constructor initializes all elements to value", "[matrix][constructor]") {
      Matrix m(2, 3, 5.5);

      REQUIRE(m.getRows() == 2);
      REQUIRE(m.getCols() == 3);

      for (size_t i = 0; i < 2; i++) {
          for (size_t j = 0; j < 3; j++) {
              CHECK(m(i, j) == 5.5);
          }
      }
  }

  TEST_CASE("Fill constructor with negative value", "[matrix][constructor]") {
      Matrix m(2, 2, -3.14);

      CHECK(m(0, 0) == -3.14);
      CHECK(m(0, 1) == -3.14);
      CHECK(m(1, 0) == -3.14);
      CHECK(m(1, 1) == -3.14);
  }

  TEST_CASE("Fill constructor with zero", "[matrix][constructor]") {
      Matrix m(2, 2, 0.0);

      for (size_t i = 0; i < 2; i++) {
          for (size_t j = 0; j < 2; j++) {
              CHECK(m(i, j) == 0.0);
          }
      }
  }

  TEST_CASE("Initializer list constructor", "[matrix][constructor]") {
      Matrix m = {{1, 2, 3},
                  {4, 5, 6}};

      REQUIRE(m.getRows() == 2);
      REQUIRE(m.getCols() == 3);

      CHECK(m(0, 0) == 1);
      CHECK(m(0, 1) == 2);
      CHECK(m(0, 2) == 3);
      CHECK(m(1, 0) == 4);
      CHECK(m(1, 1) == 5);
      CHECK(m(1, 2) == 6);
  }

  TEST_CASE("Initializer list constructor single row", "[matrix][constructor]") {
      Matrix m = {{1, 2, 3, 4}};

      CHECK(m.getRows() == 1);
      CHECK(m.getCols() == 4);
      CHECK(m(0, 2) == 3);
  }

  TEST_CASE("Initializer list constructor single column", "[matrix][constructor]") {
      Matrix m = {{1},
                  {2},
                  {3}};

      CHECK(m.getRows() == 3);
      CHECK(m.getCols() == 1);
      CHECK(m(1, 0) == 2);
  }

  TEST_CASE("Initializer list constructor 1x1", "[matrix][constructor]") {
      Matrix m = {{42}};

      CHECK(m.getRows() == 1);
      CHECK(m.getCols() == 1);
      CHECK(m(0, 0) == 42);
  }

  TEST_CASE("Initializer list rejects jagged arrays", "[matrix][constructor]") {
      CHECK_THROWS_AS((Matrix{{1, 2, 3}, {4, 5}}), std::invalid_argument);
  }

  TEST_CASE("Element access and modification", "[matrix][access]") {
      Matrix m(2, 2, 0.0);

      m(0, 0) = 1.0;
      m(0, 1) = 2.0;
      m(1, 0) = 3.0;
      m(1, 1) = 4.0;

      CHECK(m(0, 0) == 1.0);
      CHECK(m(0, 1) == 2.0);
      CHECK(m(1, 0) == 3.0);
      CHECK(m(1, 1) == 4.0);
  }

  // ==================== ADDITION ====================

  TEST_CASE("Matrix addition", "[matrix][operator]") {
      Matrix a = {{1, 2},
                  {3, 4}};
      Matrix b = {{5, 6},
                  {7, 8}};

      Matrix c = a + b;

      CHECK(c(0, 0) == 6);
      CHECK(c(0, 1) == 8);
      CHECK(c(1, 0) == 10);
      CHECK(c(1, 1) == 12);
  }

  TEST_CASE("Matrix addition does not modify operands", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = {{5, 6}, {7, 8}};

      Matrix c = a + b;

      CHECK(a(0, 0) == 1);  // a unchanged
      CHECK(b(0, 0) == 5);  // b unchanged
  }

  TEST_CASE("Matrix addition dimension mismatch throws", "[matrix][operator]") {
      Matrix a(2, 3);
      Matrix b(2, 4);

      CHECK_THROWS_AS(a + b, std::invalid_argument);
  }

  TEST_CASE("Matrix += modifies in place", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = {{1, 1}, {1, 1}};

      a += b;

      CHECK(a(0, 0) == 2);
      CHECK(a(0, 1) == 3);
      CHECK(a(1, 0) == 4);
      CHECK(a(1, 1) == 5);
  }

  TEST_CASE("Matrix += chaining", "[matrix][operator]") {
      Matrix a = {{1, 1}, {1, 1}};
      Matrix b = {{2, 2}, {2, 2}};
      Matrix c = {{3, 3}, {3, 3}};

      a += b += c;  // b becomes {5,5}, then a becomes {6,6}

      CHECK(b(0, 0) == 5);
      CHECK(a(0, 0) == 6);
  }

  // ==================== SUBTRACTION ====================

  TEST_CASE("Matrix subtraction", "[matrix][operator]") {
      Matrix a = {{5, 6},
                  {7, 8}};
      Matrix b = {{1, 2},
                  {3, 4}};

      Matrix c = a - b;

      CHECK(c(0, 0) == 4);
      CHECK(c(0, 1) == 4);
      CHECK(c(1, 0) == 4);
      CHECK(c(1, 1) == 4);
  }

  TEST_CASE("Matrix subtraction with negatives", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = {{5, 6}, {7, 8}};

      Matrix c = a - b;

      CHECK(c(0, 0) == -4);
      CHECK(c(1, 1) == -4);
  }

  TEST_CASE("Matrix -= modifies in place", "[matrix][operator]") {
      Matrix a = {{5, 5}, {5, 5}};
      Matrix b = {{1, 2}, {3, 4}};

      a -= b;

      CHECK(a(0, 0) == 4);
      CHECK(a(0, 1) == 3);
      CHECK(a(1, 0) == 2);
      CHECK(a(1, 1) == 1);
  }

  // ==================== MATRIX MULTIPLICATION ====================

  TEST_CASE("Matrix multiplication 2x3 * 3x2", "[matrix][operator]") {
      Matrix a = {{1, 2, 3},
                  {4, 5, 6}};  // 2x3
      Matrix b = {{7, 8},
                  {9, 10},
                  {11, 12}};   // 3x2

      Matrix c = a * b;        // Should be 2x2

      REQUIRE(c.getRows() == 2);
      REQUIRE(c.getCols() == 2);

      // Row 0: [1,2,3] dot [7,9,11] = 58, [1,2,3] dot [8,10,12] = 64
      CHECK(c(0, 0) == 58);
      CHECK(c(0, 1) == 64);
      // Row 1: [4,5,6] dot [7,9,11] = 139, [4,5,6] dot [8,10,12] = 154
      CHECK(c(1, 0) == 139);
      CHECK(c(1, 1) == 154);
  }

  TEST_CASE("Matrix multiplication 2x2 * 2x2", "[matrix][operator]") {
      Matrix a = {{1, 2},
                  {3, 4}};
      Matrix b = {{5, 6},
                  {7, 8}};

      Matrix c = a * b;

      CHECK(c(0, 0) == 19);   // 1*5 + 2*7
      CHECK(c(0, 1) == 22);   // 1*6 + 2*8
      CHECK(c(1, 0) == 43);   // 3*5 + 4*7
      CHECK(c(1, 1) == 50);   // 3*6 + 4*8
  }

  TEST_CASE("Matrix multiplication with identity", "[matrix][operator]") {
      Matrix a = {{1, 2},
                  {3, 4}};
      Matrix identity = {{1, 0},
                         {0, 1}};

      Matrix c = a * identity;

      CHECK(c(0, 0) == 1);
      CHECK(c(0, 1) == 2);
      CHECK(c(1, 0) == 3);
      CHECK(c(1, 1) == 4);
  }

  TEST_CASE("Matrix multiplication row vector * column vector", "[matrix][operator]") {
      Matrix row = {{1, 2, 3}};     // 1x3
      Matrix col = {{4}, {5}, {6}}; // 3x1

      Matrix result = row * col;    // 1x1 (dot product)

      REQUIRE(result.getRows() == 1);
      REQUIRE(result.getCols() == 1);
      CHECK(result(0, 0) == 32);    // 1*4 + 2*5 + 3*6
  }

  TEST_CASE("Matrix multiplication column vector * row vector", "[matrix][operator]") {
      Matrix col = {{1}, {2}};      // 2x1
      Matrix row = {{3, 4, 5}};     // 1x3

      Matrix result = col * row;    // 2x3 (outer product)

      REQUIRE(result.getRows() == 2);
      REQUIRE(result.getCols() == 3);
      CHECK(result(0, 0) == 3);
      CHECK(result(0, 2) == 5);
      CHECK(result(1, 0) == 6);
      CHECK(result(1, 2) == 10);
  }

  TEST_CASE("Matrix multiplication dimension mismatch throws", "[matrix][operator]") {
      Matrix a(2, 3);
      Matrix b(4, 2);  // 3 != 4, can't multiply

      CHECK_THROWS_AS(a * b, std::invalid_argument);
  }

  // ==================== SCALAR MULTIPLICATION ====================

  TEST_CASE("Matrix * scalar", "[matrix][operator]") {
      Matrix a = {{1, 2},
                  {3, 4}};

      Matrix b = a * 2.0;

      CHECK(b(0, 0) == 2);
      CHECK(b(0, 1) == 4);
      CHECK(b(1, 0) == 6);
      CHECK(b(1, 1) == 8);
  }

  TEST_CASE("Matrix * scalar does not modify original", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = a * 2.0;

      CHECK(a(0, 0) == 1);  // unchanged
  }

  TEST_CASE("Matrix * scalar with zero", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = a * 0.0;

      CHECK(b(0, 0) == 0);
      CHECK(b(1, 1) == 0);
  }

  TEST_CASE("Matrix * scalar with negative", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = a * -1.0;

      CHECK(b(0, 0) == -1);
      CHECK(b(1, 1) == -4);
  }

  TEST_CASE("scalar * Matrix (friend)", "[matrix][operator]") {
      Matrix a = {{1, 2},
                  {3, 4}};

      Matrix b = 3.0 * a;

      CHECK(b(0, 0) == 3);
      CHECK(b(0, 1) == 6);
      CHECK(b(1, 0) == 9);
      CHECK(b(1, 1) == 12);
  }

  TEST_CASE("scalar * Matrix equals Matrix * scalar", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};

      Matrix b = a * 2.5;
      Matrix c = 2.5 * a;

      CHECK(b(0, 0) == c(0, 0));
      CHECK(b(0, 1) == c(0, 1));
      CHECK(b(1, 0) == c(1, 0));
      CHECK(b(1, 1) == c(1, 1));
  }

  TEST_CASE("Matrix *= scalar modifies in place", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};

      a *= 3.0;

      CHECK(a(0, 0) == 3);
      CHECK(a(0, 1) == 6);
      CHECK(a(1, 0) == 9);
      CHECK(a(1, 1) == 12);
  }

  // ==================== EDGE CASES ====================

  TEST_CASE("Operations on 1x1 matrices", "[matrix][operator]") {
      Matrix a = {{5}};
      Matrix b = {{3}};

      CHECK((a + b)(0, 0) == 8);
      CHECK((a - b)(0, 0) == 2);
      CHECK((a * b)(0, 0) == 15);
      CHECK((a * 2.0)(0, 0) == 10);
  }

  TEST_CASE("Chained operations", "[matrix][operator]") {
      Matrix a = {{1, 2}, {3, 4}};
      Matrix b = {{1, 1}, {1, 1}};

      // (a + b) * 2
      Matrix c = (a + b) * 2.0;

      CHECK(c(0, 0) == 4);   // (1+1)*2
      CHECK(c(0, 1) == 6);   // (2+1)*2
      CHECK(c(1, 0) == 8);   // (3+1)*2
      CHECK(c(1, 1) == 10);  // (4+1)*2
  }

