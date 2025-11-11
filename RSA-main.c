#include <stdio.h>
#include <stdlib.h>
#include "RSA-functions.h"

void Usage(char *argv[])
{
    printf("Usage: %s <Prime number> <Prime number>\n", argv[0]);
    exit(EXIT_FAILURE);
}

void printArray(mpz_t *arr, size_t arr_size)
{
    size_t i;

    for(i = 0; i < arr_size; i++)
    {
        gmp_printf("%Zd\n", arr[i]);
    }
}

void freeArray(mpz_t *arr, size_t arr_size)
{
    for(size_t i = 0; i < arr_size; i++)
    {
        mpz_clear(arr[i]);
    }
}

int main(int argc, char **argv)
{
    mpz_t n, *consecutive, *L;
    size_t cons_size, L_size;

    /*
    if(argc != 3)
    {
        Usage(argv);
    }
    */

    mpz_init(n);
    gmp_scanf("%Zd", n);

    consecutive = Consecutive(n, &cons_size);
    L = Sieve(n, &L_size);

    printf("\n");
    printArray(consecutive, cons_size);
    printf("\n");
    printArray(L, L_size);

    freeArray(consecutive, cons_size);
    freeArray(L, L_size);

    free(consecutive);
    free(L);

    return 0;
}