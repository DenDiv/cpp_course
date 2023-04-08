#include <algorithm>
#include <iostream>

template <typename T>
class Matrix
{
private:
    T det_rec(const T* matrix_subbuf, const int dim) const
    {
        if (dim == 3)
        {
            T val1 = matrix_subbuf[0] * (matrix_subbuf[4] * matrix_subbuf[8] - matrix_subbuf[5] * matrix_subbuf[7]);
            T val2 = -matrix_subbuf[1] * (matrix_subbuf[3] * matrix_subbuf[8] - matrix_subbuf[5] * matrix_subbuf[6]);
            T val3 = matrix_subbuf[2] * (matrix_subbuf[3] * matrix_subbuf[7] - matrix_subbuf[4] * matrix_subbuf[6]);
            return val1 + val2 + val3;
        }

        T res = T{};
        int submatr_dim = dim - 1;
        T* new_subbuf = new T[submatr_dim * submatr_dim];

        for (int col_idx = 0; col_idx < dim; ++col_idx)
        {
            int new_idx = 0;
            int idx = dim;
            int next_skip_idx = col_idx + dim;

            // copy submatrix
            while (idx != dim * dim)
            {
                if (idx != next_skip_idx)
                {
                    new_subbuf[new_idx] = matrix_subbuf[idx];
                    new_idx++;
                }
                else
                {
                    next_skip_idx = next_skip_idx + dim;
                }
                idx++;
            }
            res = res + matrix_subbuf[col_idx] * static_cast<T>(std::pow(-1, col_idx)) * det_rec(new_subbuf, submatr_dim);
        }
        delete[] new_subbuf;
        return res;
    }

    T* matrix_buff_;
    int nrows_;
    int ncols_;

public:
    Matrix() = default;

    ~Matrix() { delete[] matrix_buff_; }

    Matrix(int nrows, int ncols, T val = T{}) : nrows_(nrows), ncols_(ncols)
    {
        matrix_buff_ = new T[nrows_ * ncols_];
        std::fill(matrix_buff_, matrix_buff_ + nrows_ * ncols_, val);
    }

    // copy constr
    Matrix(const Matrix& rhs)
    {
        nrows_ = rhs.nrows_;
        ncols_ = rhs.ncols_;
        matrix_buff_ = new T[nrows_ * ncols_];
        std::copy(rhs.matrix_buff_, rhs.matrix_buff_ + nrows_ * ncols_, matrix_buff_);
    }

    // move constr
    Matrix(Matrix&& rhs)
    {
        nrows_ = rhs.nrows_;
        ncols_ = rhs.ncols_;
        matrix_buff_ = rhs.matrix_buff_;
        rhs.matrix_buff_ = nullptr;
    }

    // copy assignment
    Matrix& operator=(const Matrix& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        nrows_ = rhs.nrows_;
        ncols_ = rhs.ncols_;
        delete matrix_buff_;
        matrix_buff_ = new T[nrows_ * ncols_];
        std::copy(rhs.matrix_buff_, rhs.matrix_buff_ + nrows_ * ncols_, matrix_buff_);
        return *this;
    }

    // move assignment
    Matrix& operator=(Matrix&& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        nrows_ = rhs.nrows_;
        ncols_ = rhs.ncols_;
        std::swap(matrix_buff_, rhs.matrix_buff_);
        return *this;
    }

    template <typename It>
    static Matrix create(int nrows, int ncols, It start, It fin)
    {
        Matrix m{};
        m.nrows_ = nrows;
        m.ncols_ = ncols;
        m.matrix_buff_ = new T[m.nrows_ * m.ncols_];
        std::copy(start, fin, m.matrix_buff_);
        return m;
    }

    void print() const
    {
        for (int i = 0; i < nrows_; ++i)
        {
            for (int j = 0; j < ncols_; ++j)
            {
                int idx = i * ncols_ + j;
                std::cout << matrix_buff_[idx] << " ";
            }
            std::cout << std::endl;
        }
    }

    int get_ncols() const { return ncols_; }

    int get_nrows() const { return nrows_; }

    bool equal(const Matrix& other) const
    {
        if (nrows_ != other.nrows_ || ncols_ != other.ncols_)
        {
            return false;
        }

        for (int i = 0; i < nrows_; ++i)
        {
            for (int j = 0; j < ncols_; ++j)
            {
                int idx = i * ncols_ + j;
                if (matrix_buff_[idx] != other.matrix_buff_[idx])
                {
                    return false;
                }
            }
        }
        return true;
    }

    Matrix& transpose() &
    {
        for (int i = 0; i < nrows_; ++i)
        {
            for (int j = i + 1; j < ncols_; ++j)
            {
                int fst_idx = i * ncols_ + j;
                int sec_idx = j * nrows_ + i;
                std::swap(matrix_buff_[fst_idx], matrix_buff_[sec_idx]);
            }
        }
        std::swap(nrows_, ncols_);
        return *this;
    }

    T det() const
    {
        if (nrows_ != ncols_)
        {
            throw std::runtime_error("nrows != ncols");
        }

        T res;
        int dim = nrows_;
        if (dim == 1)
        {
            res = matrix_buff_[0];
        }
        else if (dim == 2)
        {
            res = matrix_buff_[0] * matrix_buff_[3] - matrix_buff_[1] * matrix_buff_[2];
        }
        else
        {
            res = det_rec(matrix_buff_, dim);
        }
        return res;
    }
};