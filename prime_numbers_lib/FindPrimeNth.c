#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "FindPrimeNth.h"

typedef enum
{
    false,
    true
} bool;
#define TABLE_SIZE 1000000

bool isPrimeDummy(unsigned long long n)
{
    assert(n > 0);
    if (n == 1)
        return true;

    for (unsigned long long i = 2; i <= sqrt(n); ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

void print_erat_table(char *p, int num_bytes)
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

unsigned long long FindPrimeErat(int n)
{
    assert(n > 0);
    if (n == 1)
        return 1;
        
    int num_bytes = (TABLE_SIZE - 1) / 8 + !!((TABLE_SIZE - 1) % 8);
    char *p = (char *)malloc(sizeof(char) * num_bytes);
    memset(p, 0xFF, sizeof(char) * num_bytes);

    int prime_counter = 1;
    int byte_num;
    unsigned long long val_1, val_2;
    char mask;

    val_1 = 2;
    while (val_1 <= TABLE_SIZE)
    {
        byte_num = (val_1 - 1) / 8 + !!((val_1 - 1) % 8) - 1;
        mask = 0x80 >> (val_1 - 2) % 8;
        if (!!(p[byte_num] & mask))
        {
            prime_counter++;
            if (prime_counter == n){
                return val_1;
            }

            val_2 = val_1 * val_1;
            while (val_2 <= TABLE_SIZE)
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
    // print_erat_table(p, num_bytes);
}

unsigned long long FindPrimeDummy(int n)
{
    unsigned long long i = 1;
    int prime_counter = 0;

    while (prime_counter < n)
    {
        if (isPrimeDummy(i))
        {
            prime_counter++;
        }
        i++;
    }
    i--;
    printf("Prime number %d: %llu\n", prime_counter, i);
    return i;
}
