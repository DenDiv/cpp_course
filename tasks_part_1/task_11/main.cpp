#include <iostream>
#include <cstring>
#include <stdexcept>

char **fit_matrix(char *str, size_t N)
{
    if (std::strlen(str) != N * N)
    {
        throw std::length_error("N*N != str_len");
    }

    char **matrix = new char *[N];
    for (size_t i = 0; i < N; ++i)
    {
        matrix[i] = new char[N];
        for (size_t j = 0; j < N; ++j)
        {
            matrix[i][j] = str[i * N + j];
        }
    }

    // printing
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < N; ++j)
        {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return matrix;
}

int main()
{
    /*
    task 11. Program converts string to matrix
    */

    constexpr size_t N = 5;
    char str[N * N + 1] = "1234512345123451234512345";
    fit_matrix(str, N);
}