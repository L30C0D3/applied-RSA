#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "RSA-functions.h"

/*
    *** AUXILIARY FUNCTIONS ***
*/

// Returns an array with the binary representation of a number n
unsigned int *dec_to_bin(const mpz_t dec_number, size_t *bit_size)
{
    mpz_t temp; // Copy of dec_number for its manipulation
    unsigned int *binary_array; // Array with dec_numbers's binary representation
    size_t i;

    mpz_init(temp);
    mpz_set(temp, dec_number);

    *bit_size = mpz_sizeinbase(dec_number, 2);
    binary_array = (unsigned int *) calloc(*bit_size, sizeof(unsigned int));

    if(!binary_array)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
 
    for(i = 0; i < *bit_size; i++)
    {
        binary_array[*bit_size-i-1] = mpz_tstbit(temp, i);
    }

    mpz_clear(temp);

    return binary_array;
}

// Returns an array with consecutive numbers from 2 to n
mpz_t *Consecutive(const mpz_t n, size_t *arr_size)
{
    mpz_t *consecutive; // Array with consecutive numbers
    mpz_t current; // Variable that carries the consecutive values
    size_t i;

    *arr_size = mpz_get_ui(n) - 1;

    consecutive = (mpz_t *) calloc(*arr_size, sizeof(mpz_t));

    if(!consecutive)
    {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    mpz_init_set_ui(current, 2);

    for(i = 0; i < *arr_size; i++)
    {
        mpz_init(consecutive[i]);
        mpz_set(consecutive[i], current);
        mpz_add_ui(current, current, 1);
    }

    mpz_clear(current);

    return consecutive;
}

// Sieve of Eratosthenes. Given an array with consecutive numbers, returns an array P with prime numbers
mpz_t *Sieve(mpz_t n, size_t *L_size)
{
    mpz_t *P; // Array P with consecutive numbers from 2 to n
    mpz_t *L; // Array L with prime numbers
    size_t P_size, max_P_size, i, j;

    P = Consecutive(n, &P_size);
    *L_size = 0;

    // Process of marking multiples of P[i]
    for(i = 0; i < P_size; i++)
    {
        if(mpz_cmp_ui(P[i], 0) != 0)
        {
            j = mpz_get_ui(P[i]) * mpz_get_ui(P[i]) - 2; // We substract 2 from p^2 because P starts from 2, so it is 2 units ahead of i

            while(j < P_size)
            {
                mpz_set_ui(P[j], 0);
                j = j + mpz_get_ui(P[i]);
            }

            (*L_size)++; // This allows us to count the prime numbers in P
        }
    }

    L = (mpz_t *) calloc(*L_size, sizeof(mpz_t));

    // Process of copying prime numbers from P to L
    j = 0;
    for(i = 0; i < P_size; i++)
    {
        if(mpz_cmp_ui(P[i], 0) != 0)
        {
            mpz_init(L[j]);
            mpz_set(L[j], P[i]);
            j++;
        }
    }

    // Free up memory from P
    for(i = 0; i < P_size; i++)
    {
        mpz_clear(P[i]);
    }
    free(P);

    return L;
}

// Calculates the prime factors and its exponents given a number n using the trial division
void TrialDivision(const mpz_t n, mpz_t *primes, size_t prime_count, mpz_t ***factors, size_t *factor_count)
{

}

// Free up memory from the factor and exponents matrix
void FreeFactorMatrix(mpz_t **matrix, size_t count)
{

}


/*
    *** CALCULATION FUNCTIONS ***
*/

// Euclidian algorithm for calculating gcd(a,b)
void euclid(const mpz_t a, const mpz_t b, mpz_t gcd)
{
    mpz_t r, temp_a, temp_b;

    mpz_init(temp_a);
    mpz_init(temp_b);
    mpz_init(r);

    mpz_set(temp_a, a);
    mpz_set(temp_b, b);

    // Euclidian algorithm
    while(mpz_cmp_ui(temp_b, 0) == FALSE)
    {
        mpz_mod(r, temp_a, temp_b); // r gets the result of 'a mod b'
        mpz_set(temp_a, temp_b);
        mpz_set(temp_b, r);
    }

    // gcd gets the result of gcd(a,b)
    mpz_set(gcd, temp_a); 

    // Free memory
    mpz_clear(temp_a);
    mpz_clear(temp_b);
    mpz_clear(r);
}

// Calculates a^b using left-to-right exponentiation using Horner's rule
void bin_exp_LR(const mpz_t base, const unsigned int *bin_exp, size_t size, mpz_t result)
{
    mpz_t R;

    mpz_init_set(R, base); // R <- a

    for(size_t i = 0; i < size; i++)
    {
        mpz_mul(R, R, R); // R <- R * R

        if(bin_exp[i] == 1);
        {
            mpz_mul(R, R, base); // R <- R * a
        }
    }

    mpz_set(result, R);

    mpz_clear(R);
}

// Factorizes a number n on its prime factors
void Factorize(mpz_t n)
{

}

// Calculates Euler's phi function
void phi_euler(mpz_t m)
{
    
}