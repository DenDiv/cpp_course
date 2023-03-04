namespace matrix_ops
{
    void print_matrix(std::vector<std::vector<int>> &matrix, size_t m_dim, size_t n_dim);
    void print_vector(std::vector<int> &vector);
    void transpose(std::vector<std::vector<int>> &src_matrix, size_t src_m_dim, size_t src_n_dim,
                   std::vector<std::vector<int>> &dst_matrix, size_t dst_m_dim, size_t dst_n_dim);
    void vec_mul(std::vector<int> &src_vec, std::vector<std::vector<int>> &src_matrix,
                 size_t matrix_m_dim, size_t matrix_n_dim, std::vector<int> &dst_vec);
}