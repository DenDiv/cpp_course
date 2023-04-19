#include <algorithm>
#include <iostream>

template <typename T>
class Matrix
{
private:
    struct ProxyMatrix
    {
        T* matrix_buff_ref;
        int matrix_ncols;

        T& operator[](int n)
        {
            assert(n < matrix_ncols);
            return matrix_buff_ref[n];
        }

        const T& operator[](int n) const
        {
            assert(n < matrix_ncols);
            return matrix_buff_ref[n];
        }
    };

public:
    Matrix(int nrows, int ncols, T val = T{}) : nrows_(nrows), ncols_(ncols)
    {
        matrix_buff_ = new T[nrows_ * ncols_];
        std::fill(matrix_buff_, matrix_buff_ + nrows_ * ncols_, val);
    }

    template <typename It>
    Matrix(int nrows, int ncols, It first, It last) : nrows_(nrows), ncols_(ncols)
    {
        matrix_buff_ = new T[nrows_ * ncols_];
        std::copy(first, last, matrix_buff_);
    }

    ~Matrix() { delete[] matrix_buff_; }

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
        swap(*this, rhs);
        rhs.matrix_buff_ = nullptr;
    }

    // copy assignment
    Matrix& operator=(const Matrix& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        Matrix tmp(rhs);
        swap(*this, rhs);
        return *this;
    }

    // move assignment
    Matrix& operator=(Matrix&& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }

        swap(*this, rhs);
        rhs.matrix_buff_ = nullptr;
        return *this;
    }

    ProxyMatrix operator[](int n)
    {
        assert(n < nrows_);
        return ProxyMatrix{matrix_buff_ + n * ncols_, ncols_};
    }

    const ProxyMatrix operator[](int n) const
    {
        assert(n < nrows_);
        return ProxyMatrix{matrix_buff_ + n * ncols_, ncols_};
    }

    Matrix& operator+=(const Matrix& rhs)
    {
        for (int i = 0, i_end = nrows_ * ncols_; i < i_end; ++i)
        {
            matrix_buff_[i] += rhs.matrix_buff_[i];
        }
        return *this;
    }

    Matrix& operator+=(T rhs)
    {
        for (int i = 0, i_end = nrows_ * ncols_; i < i_end; ++i)
        {
            matrix_buff_[i] += rhs;
        }
        return *this;
    }

public:

    friend void swap(Matrix& first, Matrix& second)
    {
        std::swap(first.nrows_, second.nrows_);
        std::swap(first.ncols_, second.ncols_);
        std::swap(first.matrix_buff_, second.matrix_buff_);
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
        if (&other == this)
        {
            return true;
        }

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
};

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    Matrix<T> tmp(lhs);
    tmp += rhs;
    return tmp;
}

template <typename T>
Matrix<T> operator+(const T lhs, const Matrix<T>& rhs)
{
    Matrix<T> tmp(rhs);
    tmp += lhs;
    return tmp;
}

template <typename T>
Matrix<T> operator+(const Matrix<T>& lhs, const T rhs)
{
    Matrix<T> tmp(lhs);
    tmp += rhs;
    return tmp;
}

template <typename T>
bool operator==(const Matrix<T>& lhs, const Matrix<T>& rhs)
{
    return lhs.equal(rhs);
}
