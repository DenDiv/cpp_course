#include <iostream>
#include <vector>
#include "MatrixOps.h"

void matrix_ops::print_matrix(std::vector<std::vector<int>> &matrix, size_t m_dim, size_t n_dim)
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

void matrix_ops::print_vector(std::vector<int> &vector)
{
    for (int i : vector)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void matrix_ops::transpose(std::vector<std::vector<int>> &src_matrix, size_t src_m_dim, size_t src_n_dim,
                           std::vector<std::vector<int>> &dst_matrix, size_t dst_m_dim, size_t dst_n_dim)
{
    if (src_m_dim != dst_n_dim || src_n_dim != dst_m_dim)
    {
        throw std::length_error("src_m_dim != dst_n_dim or src_n_dim != dst_m_dim");
    }

    for (size_t i = 0; i < src_m_dim; ++i)
    {
        for (size_t j = i; j < src_n_dim; ++j)
        {
            dst_matrix[i][j] = src_matrix[j][i];
            dst_matrix[j][i] = src_matrix[i][j];
        }
    }
}

void matrix_ops::vec_mul(std::vector<int> &src_vec, std::vector<std::vector<int>> &src_matrix,
                         size_t matrix_m_dim, size_t matrix_n_dim, std::vector<int> &dst_vec)
{
    if (matrix_m_dim != src_vec.size())
    {
        throw std::length_error("matrix_m_dim != src_vec.size()");
    }

    if (matrix_n_dim != dst_vec.size())
    {
        throw std::length_error("matrix_n_dim != dst_vec.size()");
    }

    int tmp_val = 0;
    for (size_t i = 0; i < matrix_n_dim; ++i)
    {
        for (size_t j = 0; j < matrix_m_dim; ++j)
        {
            tmp_val += src_vec[j] * src_matrix[j][i];
        }
        dst_vec[i] = tmp_val;
        tmp_val = 0;
    }
}