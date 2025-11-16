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
    mpz_t n, phi_result;
    /*
    if(argc != 3)
    {
        Usage(argv);
    }
    */

    mpz_init(n);
    mpz_init(phi_result);

    // 2. Pedir el número al usuario
    gmp_printf("Introduce un número para calcular phi(n): ");
    gmp_scanf("%Zd", n);

    phi_euler(n, phi_result);

    gmp_printf("\nphi(%Zd) = %Zd\n", n, phi_result);

    mpz_clear(n);
    mpz_clear(phi_result);
    
    return 0;
}