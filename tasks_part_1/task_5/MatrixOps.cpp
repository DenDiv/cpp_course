#include <iostream>
#include "MatrixOps.h"

matrix_tag::matrix_tag(int **matrix, size_t m_dim, size_t n_dim)
{
    // copy matrix
    this->matrix = new int *[m_dim];
    for (size_t i = 0; i < m_dim; ++i)
    {
        this->matrix[i] = new int[n_dim];
        for (size_t j = 0; j < n_dim; ++j)
        {
            this->matrix[i][j] = matrix[i][j];
        }
    }
    this->m_dim = m_dim;
    this->n_dim = n_dim;
}

matrix_tag::matrix_tag(size_t m_dim, size_t n_dim)
{
    this->m_dim = m_dim;
    this->n_dim = n_dim;
}

void matrix_tag::print()
{
    for (size_t i = 0; i < m_dim; ++i)
    {
        for (size_t j = 0; j < n_dim; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

matrix_p matrix_tag::transpose()
{
    int **transposed_matrix = new int *[m_dim];
    for (size_t i = 0; i < m_dim; ++i)
    {
        transposed_matrix[i] = new int[n_dim];
    }

    for (size_t i = 0; i < m_dim; ++i)
    {
        for (size_t j = i; j < n_dim; ++j)
        {
            transposed_matrix[i][j] = matrix[j][i];
            transposed_matrix[j][i] = matrix[i][j];
        }
    }
    matrix_p matrix_struct = new matrix_t(m_dim, n_dim);
    matrix_struct->matrix = transposed_matrix;
    return matrix_struct;
}

int *matrix_tag::vec_mul(int *vector, size_t vec_size)
{
    if (vec_size != m_dim)
    {
        throw std::length_error("vec_size isn't equal to m_dim");
    }

    int *res_vec = new int[n_dim];
    int tmp_val = 0;
    for (size_t i = 0; i < n_dim; ++i)
    {
        for (size_t j = 0; j < vec_size; ++j)
        {
            tmp_val += vector[j] * matrix[j][i];
        }
        res_vec[i] = tmp_val;
        tmp_val = 0;
    }
    return res_vec;
}