#include <iostream>
#include <vector>
#include "MatrixOps.h"

int main()
{
    /* Matrix transpose */

    // allocate memory for source matrix
    size_t src_m_dim = 3, src_n_dim = 3;
    std::vector<std::vector<int>> src_matrix(src_n_dim, std::vector<int>(src_m_dim));
    for (size_t i = 0; i < src_m_dim; ++i)
    {
        for (size_t j = 0; j < src_n_dim; ++j)
        {
            src_matrix[i][j] = i * src_m_dim + j;
        }
    }

    // allocate memory for destination matrix
    size_t dst_m_dim = 3, dst_n_dim = 3;
    std::vector<std::vector<int>> dst_matrix(src_n_dim, std::vector<int>(dst_m_dim));

    // print src matrix
    std::cout << "Source matrix: " << std::endl;
    matrix_ops::print_matrix(src_matrix, src_m_dim, src_n_dim);

    // print transposed matrix
    std::cout << std::endl
              << "Transpose result matrix: " << std::endl;
    matrix_ops::transpose(src_matrix, src_m_dim, src_n_dim,
                          dst_matrix, dst_m_dim, dst_n_dim);
    matrix_ops::print_matrix(dst_matrix, dst_m_dim, dst_n_dim);

    /* Vec Mul Results */
    std::vector<int> src_vec(src_m_dim, 1);
    std::vector<int> dst_vec(src_n_dim);

    // print src vector
    std::cout << std::endl
              << "Source vec: ";
    matrix_ops::print_vector(src_vec);

    // print mult result
    matrix_ops::vec_mul(src_vec, src_matrix, src_m_dim, src_n_dim, dst_vec);
    std::cout << "Mul results: ";
    matrix_ops::print_vector(dst_vec);
    return 0;
}
