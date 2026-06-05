#include "matrix.hpp"

#include <iostream>
#include <cmath>
#include <stdexcept>

// Default constructor creates an empty matrix with 0 rows and 0 columns
Matrix::Matrix(): rows_(0), cols_(0), data_(0) 
{
}

// Allocate rows x cols and fill with 0.0
Matrix::Matrix(size_t rows, size_t cols)
    : rows_(rows), cols_(cols), data_(rows * cols, 0.0)
{  
}

Matrix::Matrix(size_t rows, size_t cols, double value)
    : rows_(rows), cols_(cols), data_(rows * cols, value)
{
}

Matrix::Matrix(std::initializer_list<std::initializer_list<double>> values) { 

    rows_ = values.size();
    cols_ = 0;



    if (rows_ > 0) { 
        cols_ = values.begin()->size();
    }

    for (const auto& row: values) 
    { 
        if (row.size() != cols_)
        throw std::invalid_argument(
                "Matrix constructor error: jagged rows detected");
    }

    data_.resize(rows_ * cols_);

    size_t idx = 0;

    for (const auto& row: values)
    {
        for (const auto& val: row)
        {
            data_[idx++] = val;
        }
    }
}

// do not return *this here as we are creating an entirely new matrix using addition. I.e. C = A + B
Matrix Matrix::operator+(const Matrix& other) const {

    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::invalid_argument("Matrix dimension mismatch");
    }

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }

    return result;
}

// returning *this allows for chaining of operations as we are changing the current matrix and returning it by reference, e.g. A += B += C
Matrix& Matrix::operator+=(const Matrix& other) { 

    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::invalid_argument("Matrix dimension mismatch");
    }

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            (*this)(i, j) += other(i, j);
        }
    }

    return *this;

}

Matrix Matrix::operator-(const Matrix& other) const { 

    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::invalid_argument("Matrix dimension mismatch");
    }
    
    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i ,j) - other(i, j);
        }
    }

    return result;
}

Matrix& Matrix::operator-=(const Matrix& other) { 

    if (rows_ != other.rows_ || cols_ != other.cols_) {
      throw std::invalid_argument("Matrix dimension mismatch");
    }  

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            (*this)(i, j) -= other(i, j);
        }
    }

    return *this;
}

//  posssibly could be ++i as opposed to i++
Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrix multiplication error: incompatible dimensions");
    }

    Matrix result(rows_, other.cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < other.cols_; j++) {

            double sum = 0.0;

            for (size_t k = 0; k < cols_; k++) { 
                sum += (*this)(i, k) * other(k, j);
            }

            result(i, j) = sum;
        }
    }

    return result;
}

Matrix Matrix::operator*(double scalar) const {

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) { 
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = (*this)(i, j) * scalar;
        }
    }

    return result;
}

Matrix& Matrix::operator*=(double scalar) {
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            (*this)(i, j) = (*this)(i, j) * scalar;
        }
    }
    return *this;
}

// friend function so no Matrix::... as this is a free function, not a member function
Matrix operator*(double scalar, const Matrix& m) {
      return m * scalar;  // Just reuse the member operator
  }

  Matrix Matrix::transpose() const {
    Matrix result(cols_, rows_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(j, i) = (*this)(i, j);
        }
    }
    return result;
  }

Matrix Matrix::hadamard(const Matrix& other) const {

    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Incompatible Dimensions");
    }

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {

            result(i, j) = (*this)(i, j) * other(i, j);
        }
    }
    return result;
  }

template<typename Func>
Matrix Matrix::apply(Func func) const {

    Matrix result(rows_, cols_);

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            result(i, j) = func((*this)(i, j));
        }
    }

    return result;
}

double Matrix::sum() const {

    double total = 0.0;

    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < cols_; j++) {
            total += (*this)(i, j);
        }
    }
    return total;
}

Matrix Matrix::sum(int axis) const 
{
    
    if (axis == 0) 
    {
       Matrix result(1, cols_);

       for (size_t j = 0; j < cols_, j++) 
       {
        double total = 0.0;
        for (size_t i = 0; i < rows_; i++)
        {
            total += (*this)(i, j);
        }
        result(0, j) = total;
       }
       return result;
    }
    else if (axis == 1) 
    {
        Matrix result(rows_, 1);

        for (size_t i = 0; i < rows_; i++)
        {
            double total = 0.0;
            for (size_t j = 0; j < cols_; j++)
            {
                total += (*this)(i, j);
            }
            result(i, j) = total;
        }
        return result;
    }
    
    throw std::invalid_argument("axis must be 0 or 1");

}




//  // Matrix * scalar
//         Matrix operator*(double scalar) const;
//         Matrix& operator*=(double scalar);




