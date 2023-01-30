typedef struct matrix_tag
{
    matrix_tag(size_t m_dim, size_t n_dim);
    matrix_tag(int **matrix, size_t m_dim, size_t n_dim);
    int **matrix;
    size_t m_dim, n_dim;
    matrix_tag *transpose();
    void print();
    int *vec_mul(int *vector, size_t vec_size);
} matrix_t, *matrix_p;