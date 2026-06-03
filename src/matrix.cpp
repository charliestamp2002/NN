#include "matrix.hpp"

#include <iostream>
#include <cmath>

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

    // for (size_t r = 0; r < rows_; r++)
    // {
    //     for (size_t c = 0; c < cols_; c++)
    //     {
    //         data_[r * cols_ + c] = values.begin()[r].begin()[c];
    //     }
    // }  

}




