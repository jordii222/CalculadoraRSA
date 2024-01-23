#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>

void generate_large_prime(mpz_t prime, gmp_randstate_t r_state, int bit_count) {
    // Generate a random number
    mpz_urandomb(prime, r_state, bit_count);

    // Find the next prime number
    mpz_nextprime(prime, prime);
}

void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d) {
    // Calculate n = p * q
    mpz_mul(n, p, q);

    // Calculate phi_n = (p-1) * (q-1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);

    // Choose e such that 1 < e < phi_n and gcd(e, phi_n) = 1
    mpz_set_ui(e, 3);
    while(1) {
        mpz_t gcd;
        mpz_init(gcd);

        mpz_gcd(gcd, e, phi_n);
        if(mpz_cmp_ui(gcd, 1) == 0) {
            mpz_clear(gcd);
            break;
        }

        mpz_add_ui(e, e, 2);
        mpz_clear(gcd);
    }

    // Calculate d = e^-1 mod phi_n
    if(mpz_invert(d, e, phi_n) == 0) {
        mpz_clear(phi_n);
        gmp_printf("Modular inverse does not exist\n");
        return;
    }
}

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    // Calculate c = m^e mod n
    mpz_powm(c, m, e, n);
}

void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    // Calculate m = c^d mod n
    mpz_powm(m, c, d, n);
}

int main(void) {
    gmp_randstate_t r_state;
    mpz_t p, q, n, phi_n, e, d, m, c;
    unsigned long int seed;
    int bit_count = 1024;  // Number of bits of the prime number

    mpz_inits(p, q, n, phi_n, e, d, m, c, NULL);
    gmp_randinit_default(r_state);

    // Initialize the seed with the current time
    seed = time(NULL);
    gmp_randseed_ui(r_state, seed);

    // Generate two random prime numbers
    generate_large_prime(p, r_state, bit_count);
    generate_large_prime(q, r_state, bit_count);

    // Calculate the RSA parameters
    calculate_rsa_params(p, q, n, phi_n, e, d);

    // Encrypt and decrypt each character
    char str[] = "hello good day";
    printf("Original message: %s\n", str);
    printf("Encrypted message: ");
    for(int i = 0; i < strlen(str); i++) {
        mpz_set_ui(m, str[i]);
        encrypt(c, m, e, n);
        gmp_printf("%Zd ", c);
    }
    printf("\n");
    gmp_printf("Generated prime number p: %Zd\n", p);
    printf("\n");
    gmp_printf("Generated prime number q: %Zd\n", q);
     printf("\n");
    gmp_printf("RSA parameter n: %Zd\n", n);
    printf("\n");
    gmp_printf("RSA parameter phi_n: %Zd\n", phi_n);
     printf("\n");
    gmp_printf("RSA parameter d: %Zd\n", d); 
    printf("\nDecrypted message: ");
    for(int i = 0; i < strlen(str); i++) {
        mpz_set_ui(m, str[i]);
        encrypt(c, m, e, n);
        decrypt(m, c, d, n);
        printf("%ld", mpz_get_ui(m));
        printf("(%c)", (char)mpz_get_ui(m));

    }
    printf("\n");

    mpz_clears(p, q, n, phi_n, e, d, m, c, NULL);
    gmp_randclear(r_state);

    return 0;
}
