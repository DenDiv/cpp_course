#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define N 100000

void find_prime_erat(int);
void print_table(char *, int);

int main()
{
    find_prime_erat(N);
}

void find_prime_erat(int n)
{
    assert(n > 0);
    if (n == 1)
    {
        printf("Prime num 1: 1\n");
        printf("Total prime nums: 1\n");
        return;
    }
    int num_bytes = (n - 1) / 8 + !!((n - 1) % 8);
    char *p = (char *)malloc(sizeof(char) * num_bytes);
    memset(p, 0xFF, sizeof(char) * num_bytes);

    long long val_1, val_2;
    long long byte_num;
    char mask;

    val_1 = 2;
    while (val_1 <= N)
    {
        byte_num = (val_1 - 1) / 8 + !!((val_1 - 1) % 8) - 1;
        mask = 0x80 >> (val_1 - 2) % 8;
        if (!!(p[byte_num] & mask))
        {
            printf("Prime number: %d\n", val_1);
            val_2 = val_1 * val_1;
            while (val_2 <= N)
            {
                byte_num = (val_2 - 1) / 8 + !!((val_2 - 1) % 8) - 1;
                mask = ~(0x80 >> (val_2 - 2) % 8);
                p[byte_num] &= mask;
                val_2 += val_1;
            }
        }
        val_1++;
    }
    // printf("Binary table:\n");
    // print_table(p, num_bytes);
}

void print_table(char *p, int num_bytes)
{
    for (int i = 0; i < num_bytes; ++i)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%d", !!((p[i] << j) & 0x80));
        }
        printf("\n");
    }
}