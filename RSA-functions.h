#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

#define TRUE 1
#define FALSE 0
/*

    *** AUXILIARY FUNCTIONS ***

    - mpz_to_bin: Converts decimal numbers to its binary representation
    - Sieve: Sieve of Eratosthenes. Returns an array with prime numbers
    - TrialDivision: Calculates the prime factors and its exponents given a number n using the trial division
    - FreeFactorMatrix: Free up memory from the factor and exponents matrix
*/

unsigned int *dec_to_bin(const mpz_t dec_number, size_t *bit_size);
mpz_t *Sieve(const mpz_t sieve_limit, size_t *prime_count);
void TrialDivision(mpz_t n, mpz_t *primes, size_t primes_size, mpz_t ***factors, size_t *factor_count);
void FreeFactorMatrix(mpz_t **matrix, size_t count);
void printFactorMatrix(mpz_t **factors, size_t factors_count);

/*
    *** CALCULATION FUNCTIONS ***
    - euclid: Euclidian algorithm for calculate gcd(a,b)
    - bin_exp_LR: Calculates a^b using left-to-right exponentiation using Horner's rule
    - Factorize: Factorizes a number n on its prime factors
    - phi_euler: Calculates Euler's phi function
*/

void euclid(const mpz_t a, const mpz_t b, mpz_t gcd);
void bin_exp_LR(const mpz_t base, const unsigned int *bin_exp, size_t size, mpz_t result);
mpz_t **Factorize(mpz_t n, size_t *factors_count);
void phi_euler(mpz_t m, mpz_t result);