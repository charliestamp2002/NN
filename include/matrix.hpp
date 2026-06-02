#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

class Matrix {

    private: 
        size_t rows_;
        size_t cols_;
        std::vector<double> data_;

    public:
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


