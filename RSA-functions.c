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
        fprintf(stderr, "Memory allocation error in decimal to binary conversion\n");
        exit(EXIT_FAILURE);
    }
 
    for(i = 0; i < *bit_size; i++)
    {
        binary_array[*bit_size-i-1] = mpz_tstbit(temp, i);
    }

    mpz_clear(temp);

    return binary_array;
}

// Sieve of Eratosthenes. Returns an array with prime numbers
mpz_t *Sieve(const mpz_t sieve_limit, size_t *prime_count)
{
    char *sieve_marks; // Array with the marks of prime numbers
    mpz_t *primes; // Array with prime numbers
    size_t sieve_limit_ui, i, j, p; // Sieve limit converted to unsigned int

    if(!mpz_fits_ulong_p(sieve_limit))
    {
        fprintf(stderr, "Error: Sieve's limit is too big for unsigned int\n");
        exit(EXIT_FAILURE);
    }

    sieve_limit_ui = mpz_get_ui(sieve_limit);

    sieve_marks = (char *) calloc(sieve_limit_ui + 1, sizeof(char));
    if(!sieve_marks)
    {
        fprintf(stderr, "Memory allocation error in sieve marks array\n");
        exit(EXIT_FAILURE);
    }

    // We mark 0 and 1 as non-primes
    sieve_marks[0] = 0;
    sieve_marks[1] = 0;
    for(i = 2; i <= sieve_limit_ui; i++)
    {
        sieve_marks[i] = 1; // We assume all numbers are prime
    }

    // Executing Sieve algorithm
    for(p = 2; p*p <= sieve_limit_ui; p++)
    {
        if(sieve_marks[p] == 1)
        {
            for(i = p*p; i <= sieve_limit_ui; i = i + p)
            {
                sieve_marks[i] = 0; // If the element is a multiple of p, we mark it as non-prime
            }
        }
    }

    // Counting all the primes found in array sieve_marks
    *prime_count = 0;
    for(p = 2; p <= sieve_limit_ui; p++)
    {
        if(sieve_marks[p] == 1)
        {
            (*prime_count)++;
        }
    }

    primes = (mpz_t *) calloc(*prime_count, sizeof(mpz_t));
    if(!primes)
    {
        fprintf(stderr, "Memory allocation error in primes array\n");
        exit(EXIT_FAILURE);        
    }

    // Copy prime numbers from sieve_marks array to primes array
    j = 0;
    for(i = 2; i <= sieve_limit_ui; i++)
    {
        if(sieve_marks[i] == 1)
        {
            mpz_init_set_ui(primes[j], i);
            j++;
        }
    }

    free(sieve_marks);

    return primes;
}

// Calculates the prime factors and its exponents given a number n using the trial division
void TrialDivision(mpz_t n, mpz_t *primes, size_t primes_size, mpz_t ***factors, size_t *factor_count)
{
    mpz_t remainder, n_temp, exp_count;
    mpz_t *factor_row, **new_factors;
    size_t i;

    *factors = NULL; // Initializing NULL for realloc
    *factor_count = 0;

    mpz_init(remainder);
    mpz_init(exp_count);
    mpz_init_set(n_temp, n);

    for(i = 0; i < primes_size; i++)
    {
        // If n reduces to 1, finish
        if(mpz_cmp_ui(n_temp, 1) == 0)
        {
            break;
        }
        
        mpz_mod(remainder, n_temp, primes[i]); // Calculates n mod primes[i]

        // Verifies that P[i] divides n
        if(mpz_cmp_ui(remainder, 0) == 0)
        {
            mpz_set_ui(exp_count, 0);

            // Process of counting the factor's exponent
            do
            {
                mpz_add_ui(exp_count, exp_count, 1);
                mpz_divexact(n_temp, n_temp, primes[i]);
                mpz_mod(remainder, n_temp, primes[i]);
            }
            while(mpz_cmp_ui(remainder, 0) == 0);
                
            factor_row = (mpz_t *) calloc(2, sizeof(mpz_t)); // Allocates memory for the factor row
            if(!factor_row)
            {
                fprintf(stderr, "Memory allocation error in factors row\n");
                exit(EXIT_FAILURE);
            }

            mpz_init_set(factor_row[0], primes[i]);
            mpz_init_set(factor_row[1], exp_count);
            (*factor_count)++;

            // Reallocates memory in factors matrix
            new_factors = (mpz_t **) realloc(*factors, (*factor_count) * sizeof(mpz_t *));

            // Sets the updated matrix and sets the row to the matrix
            *factors = new_factors;
            (*factors)[*factor_count-1] = factor_row;
        }
    }

    // Process for including the last factor in the matrix. This happens because
    // in the Sieve algorithm the limit is set in sqrt(n), then the last term is omitted.
    if(mpz_cmp_ui(n_temp, 1) > 0)
    {
        factor_row = (mpz_t *) calloc(2, sizeof(mpz_t));

        mpz_init_set(factor_row[0], n_temp);
        mpz_init_set_ui(factor_row[1], 1);

        (*factor_count)++;

        new_factors = (mpz_t **) realloc(*factors, (*factor_count) * sizeof(mpz_t *));

        *factors = new_factors;
        (*factors)[*factor_count-1] = factor_row;
    }

    mpz_clear(remainder);
    mpz_clear(n_temp);
    mpz_clear(exp_count);
}

// Free up memory from the factor and exponents matrix
void FreeFactorMatrix(mpz_t **matrix, size_t size)
{
    size_t i;
    
    for(i = 0; i < size; i++)
    {
        mpz_clear(matrix[i][0]);
        mpz_clear(matrix[i][1]);
        free(matrix[i]);
    }

    free(matrix);
}

void printFactorMatrix(mpz_t **factors, size_t factor_count)
{
    size_t i;

    gmp_printf("\nFactores primos y exponentes:\n");
    gmp_printf("\nDivisor\t\tExponente\n");
    for(i = 0; i < factor_count; i++)
    {
        gmp_printf("  %Zd\t\t  %Zd\n", factors[i][0], factors[i][1]);
    }
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
    size_t i;

    mpz_init_set(R, base); // R <- a

    for(i = 1; i < size; i++)
    {
        mpz_mul(R, R, R); // R <- R * R

        if(bin_exp[i] == 1)
        {
            mpz_mul(R, R, base); // R <- R * a
        }
    }

    mpz_set(result, R);

    mpz_clear(R);
}

// Factorizes a number n on its prime factors
mpz_t **Factorize(mpz_t n, size_t *factors_count)
{
    mpz_t **factors, *primes, sieve_limit;
    size_t factor_count, primes_size, i;

    mpz_init(sieve_limit);
    mpz_sqrt(sieve_limit, n);

    primes = Sieve(sieve_limit, &primes_size); // Array that contains all prime numbers from 2 to sqrt(n)

    // Calculates the prime factors of n
    TrialDivision(n, primes, primes_size, &factors, &factor_count);

    *factors_count = factor_count;

    // Free up memory
    mpz_clear(sieve_limit);

    for(i = 0; i < primes_size; i++)
    {
        mpz_clear(primes[i]);
    }
    free(primes);

    return factors;
}


// Calculates Euler's phi function
void phi_euler(mpz_t m, mpz_t result)
{
    mpz_t **factors;
    mpz_t term, p_pow_e, p_pow_e_minus_1; // Variables for calculation
    mpz_t *p, *e; // Variables for prime factor and exponent, respectively
    unsigned long e_ui;
    size_t factors_count, i;

    // Gets the prime factorization
    factors = Factorize(m, &factors_count);

    mpz_init(term);
    mpz_init(p_pow_e);
    mpz_init(p_pow_e_minus_1);

    // Initialize result as 1 for multiplication
    mpz_set_ui(result, 1);

    for(i = 0; i < factors_count; i++)
    {
        p = &factors[i][0];
        e = &factors[i][1];

        // Convert e to ui for e-1
        e_ui = mpz_get_ui(*e);

        // Calculation of terms
        mpz_pow_ui(p_pow_e, *p, e_ui); // p^e
        mpz_pow_ui(p_pow_e_minus_1, *p, e_ui-1); // p^(e-1)
        mpz_sub(term, p_pow_e, p_pow_e_minus_1); // p^e - p^(e-1)

        mpz_mul(result, result, term);
    }

    mpz_clear(term);
    mpz_clear(p_pow_e);
    mpz_clear(p_pow_e_minus_1);

    FreeFactorMatrix(factors, factors_count);
}