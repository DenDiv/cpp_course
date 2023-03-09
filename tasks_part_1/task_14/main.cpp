#include <iostream>
#include <cmath>
#include <cstring>

void copy_array(char *src_array, char *dst_array)
{
    // make copy of src_array: dst_array = src_array
    size_t idx = 0;
    while (src_array[idx] != '\0')
    {
        dst_array[idx] = src_array[idx];
        idx++;
    }
    dst_array[idx] = '\0';
}

void add_arrays(char *val_1, char *val_2)
{
    // function sums val_1 and val_2 arrays to val_2
    // val_1 and val_2 arrays are reversed
    bool need_add_one = false;
    bool val_1_is_finished = false;
    size_t idx = 0;
    char add_res;
    while (val_2[idx] != '\0')
    {
        // val_2[idx] - 48
        add_res = val_2[idx] - '0';
        if (need_add_one)
        {
            add_res++;
        }

        if (val_1[idx] != '\0')
        {
            add_res += val_1[idx] - '0';
        }
        else
        {
            val_1_is_finished = true;
        }

        if (add_res > 9)
        {
            need_add_one = true;
            add_res %= 10;
        }
        else
        {
            need_add_one = false;
        }
        val_2[idx] = add_res + '0';

        if (val_1_is_finished && !need_add_one)
        {
            break;
        }
        idx++;
    }

    if (need_add_one)
    {
        val_2[idx] = '1';
        val_2[idx + 1] = '\0';
    }
}

char *formulaFib(size_t num_digits)
{
    char *val_1 = new char[num_digits + 1];
    char *val_2 = new char[num_digits + 1];
    char *tmp = new char[num_digits + 1];
    val_1[0] = '0';
    val_1[1] = '\0';
    val_2[0] = '1';
    val_2[1] = '\0';

    while (true)
    {
        // make copy of val_2: tmp = val_2
        copy_array(val_2, tmp);

        // val_2 = val_1 + val_2
        add_arrays(val_1, val_2);

        // val_1 = tmp
        copy_array(tmp, val_1);

        if (std::strlen(val_1) == num_digits)
        {
            break;
        }
    }
    delete [] val_2, tmp;
    return val_1;
}

int main()
{
    /*
    task 14 - Program that returns Fibonacci number with 1000 decimal digits
    */
    size_t num_digits = 1000;
    char *res;

    res = formulaFib(num_digits);
    std::cout << "Fib num with " << num_digits << " digits: ";
    for (int i = std::strlen(res) - 1; i >= 0; --i)
    {
        std::cout << res[i];
    }
    return 0;
}