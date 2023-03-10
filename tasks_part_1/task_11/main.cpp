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

int find_substr(char *full_str, char *substr)
{
    if (std::strlen(full_str) == 0 || std::strlen(substr) == 0)
    {
        throw std::length_error("full_str or substr is empty");
    }

    if (std::strlen(full_str) < std::strlen(substr))
    {
        throw std::length_error("len(full_str) < len(substr)");
    }

    size_t idx1 = 0, idx2 = 0;
    while (idx1 < std::strlen(full_str) - std::strlen(substr) + 1)
    {
        idx2 = 0;
        bool substr_exists = true;
        while (idx2 < std::strlen(substr))
        {
            if (substr[idx2] == full_str[idx1 + idx2])
            {
                idx2++;
            }
            else
            {
                substr_exists = false;
                break;
            }
        }

        if (substr_exists)
        {
            return idx1;
        }
        else
        {
            idx1++;
        }
    }
    return -1;
}

char *remove_substr(char *full_str, char *substr)
{
    if (std::strlen(full_str) == 0 || std::strlen(substr) == 0)
    {
        throw std::length_error("full_str or substr is empty");
    }

    if (std::strlen(full_str) < std::strlen(substr))
    {
        throw std::length_error("len(full_str) < len(substr)");
    }

    int substr_idx = find_substr(full_str, substr);
    size_t full_str_len = std::strlen(full_str);
    size_t substr_len = std::strlen(substr);

    while (substr_idx != -1)
    {
        for (size_t i = substr_idx; i + substr_len < full_str_len; ++i)
        {
            full_str[i] = full_str[i + substr_len];
        }
        full_str_len -= substr_len;
        full_str[full_str_len] = '\0';
        substr_idx = find_substr(full_str, substr);
    }
    return full_str;
}

int main()
{
    /*
    task 11. Program converts string to matrix
    */

    constexpr size_t N = 5;
    char str[N * N + 1] = "1234512345123451234512345";
    fit_matrix(str, N);

    /*
    task 12. Find position of first occurance of substr
    */
    char haystack[] = "Serega stal vyebivatsa so much last time";
    char needle[] = "tsa";
    int idx = find_substr(haystack, needle);
    std::cout << idx << std::endl;

    /*
    task 13. Delete all substrings needle from string haystack
    */
    char haystack_1[] = "fffoxoxoxfxo";
    char needle_1[] = "fox";
    char *res = remove_substr(haystack_1, needle_1);
    for (size_t i = 0; i < std::strlen(res); ++i)
    {
        std::cout << res[i];
    }
}