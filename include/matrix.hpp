#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {

    private: 
        size_t rows_;
        size_t cols_;
        std::vector<double> data_;

    public:
        Matrix();

        Matrix(size_t rows, size_t cols);

        Matrix(size_t rows, size_t cols, double value);

        Matrix(std::initializer_list<std::initializer_list<double>> values);

        // & here means pass py reference, inclusion of const means we promise not to modify 'other' matrix
        // return by value here as we are making an entirely new matrix as the result of addition/subtraction/multiplication. I.e. C = A + B creates a new matrix C
        Matrix operator+(const Matrix& other);
        // return by reference here as we are modifying the current matrix and returning it, this allows for chaining of operations like A += B += C
        Matrix& operator+=(const Matrix& other);

        Matrix operator-(const Matrix& other);
        Matrix& operator-=(const Matrix& other);

        // Matrix * Matrix (MatMul)
        Matrix operator*(const Matrix& other);

        // Matrix * scalar
        Matrix operator*(double scalar) const;
        Matrix& operator*=(double scalar);

        // scalar * Matrix
        friend Matrix operator*(double scalar, const Matrix& Matrix);


        double& operator()(size_t r, size_t c) {
            return data_[r * cols_ + c];
        }
        double operator()(size_t r, size_t c) const { 
            return data_[r * cols_ + c];
        }
        
        size_t getRows() const {return rows_;}
        size_t getCols() const {return cols_;}
};

#endif // MATRIX_H


