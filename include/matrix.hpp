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
        Matrix operator+(const Matrix& other) const;
        // return by reference here as we are modifying the current matrix and returning it, this allows for chaining of operations like A += B += C
        Matrix& operator+=(const Matrix& other);

        Matrix operator-(const Matrix& other) const;
        Matrix& operator-=(const Matrix& other);

        // Matrix * Matrix (MatMul)
        Matrix operator*(const Matrix& other) const;

        // Matrix * scalar
        Matrix operator*(double scalar) const;
        Matrix& operator*=(double scalar);

        // scalar * Matrix
        friend Matrix operator*(double scalar, const Matrix& m);

        double& operator()(size_t r, size_t c) {
            return data_[r * cols_ + c];
        }
        double operator()(size_t r, size_t c) const { 
            return data_[r * cols_ + c];
        }
        
        size_t getRows() const {return rows_;}
        size_t getCols() const {return cols_;}

        // const for both as we do not modify the original matrix, we return a new one
        Matrix transpose() const;
        Matrix hadamard(const Matrix& other) const;


        // apply function:
        template<typename Func>
        Matrix apply(Func func) const {

            Matrix result(rows_, cols_);

            for (size_t i = 0; i < rows_; i++) {
                for (size_t j = 0; j < cols_; j++) {
                    result(i, j) = func((*this)(i, j));
                }
            }

            return result;
        }

        double sum() const;
        Matrix sum(int axis) const;      // Sum along axis (0=columns, 1=rows)

        //  STATIC FUNCS: these funcs do not need a this pointer as it does not make sense 
        // to use a pre-existing matrix to implement these funcs

        static Matrix zeros(size_t rows, size_t cols);
        static Matrix ones(size_t rows, size_t cols);
        static Matrix random(size_t rows, size_t cols, double min = -1.0, double max = 1.0);
        static Matrix randn(size_t rows, size_t cols, double mean = 0.0, double stddev = 1.0);

};

#endif // MATRIX_H


