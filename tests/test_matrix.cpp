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


