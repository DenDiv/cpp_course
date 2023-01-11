#include <stdio.h>
#include "FindPrimeNth.h"

int main(){
    int n;
    unsigned long long prime_1, prime_2;
    printf("Enter Nth prime number: ");
    scanf("%d", &n);

    prime_1 = FindPrimeDummy(n);
    prime_2 = FindPrimeErat(n);
    printf("Result by Dummy method: %llu\n", prime_1);
    printf("Result by Erat method : %llu\n", prime_2);
}