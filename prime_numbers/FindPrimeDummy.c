#include <stdio.h>
#include <math.h>
#include <assert.h>

#define N 3000
typedef enum
{
    false,
    true
} bool;

bool is_prime(int);

int main()
{
    int i = 1;
    int prime_counter = 0;

    while (prime_counter < N)
    {
        if (is_prime(i))
        {
            prime_counter++;
            printf("Prime number %d: %d\n", prime_counter, i);
        }
        i++;
    }
}

bool is_prime(int n)
{
    assert(n > 0);
    if (n == 1)
        return true;

    for (int i = 2; i <= sqrt(n); ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}