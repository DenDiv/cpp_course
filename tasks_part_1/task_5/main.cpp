#include <iostream>
#include "MatrixOps.h"

int main()
{
    /* Matrix transpose */
    size_t m_dim = 3, n_dim = 3;
    int **source_matrix = new int *[m_dim];
    for (size_t i = 0; i < m_dim; ++i)
    {
        source_matrix[i] = new int[n_dim];
    }

    for (size_t i = 0; i < m_dim; ++i)
    {
        for (size_t j = 0; j < n_dim; ++j)
        {
            source_matrix[i][j] = i * m_dim + j;
        }
    }

    matrix_t matrix(source_matrix, m_dim, n_dim);
    matrix_p transposed_matrix;
    std::cout << "Source matrix: " << std::endl;
    matrix.print();
    std::cout << std::endl << "Result matrix: " << std::endl;
    transposed_matrix = matrix.transpose();
    (*transposed_matrix).print();

    /* Vec Mul Results */
    int *vec = new int[m_dim];
    for (size_t i = 0; i < m_dim; ++i)
    {
        vec[i] = 1;
    }

    std::cout << "Source vec: ";   
    for (size_t i = 0; i < m_dim; ++i)
    {
        std::cout << vec[i] << " ";
    }

    int *res_vec = matrix.vec_mul(vec, m_dim);
    std::cout << "Mul results: ";
    for (size_t i = 0; i < m_dim; ++i)
    {
        std::cout << res_vec[i] << " ";
    }
    return 0;
}
