#include <stdio.h>
#include <gmp.h>
#include <string.h>

int is_prime(mpz_t num) {
    return mpz_probab_prime_p(num, 25);
}

void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d) {
    mpz_t p_minus_1, q_minus_1;
    mpz_inits(p_minus_1, q_minus_1, NULL);

    // Calculate n = p * q
    mpz_mul(n, p, q);

    // Calculate phi_n = (p-1) * (q-1)
    mpz_sub_ui(p_minus_1, p, 1);
    mpz_sub_ui(q_minus_1, q, 1);
    mpz_mul(phi_n, p_minus_1, q_minus_1);

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

        mpz_add_ui(e, e, 1);
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
    mpz_t p, q, n, phi_n, e, d, m, c, p_minus_1, q_minus_1;
    char p_str[1024], q_str[1024];

    mpz_inits(p, q, n, phi_n, e, d, m, c, NULL);

    // Ask the user for the prime numbers
    printf("Enter the prime number p: ");
    scanf("%1023s", p_str);
    mpz_set_str(p, p_str, 10);
    if(!is_prime(p)) {
        printf("p is not a prime number.\n");
        return 1;
    }

    printf("Enter the prime number q: ");
    scanf("%1023s", q_str);
    mpz_set_str(q, q_str, 10);
    if(!is_prime(q)) {
        printf("q is not a prime number.\n");
        return 1;
    }

    // Calculate the RSA parameters
    calculate_rsa_params(p, q, n, phi_n, e, d);

    // Encrypt and decrypt each character
    char str[] = "This program works correctly";
    printf("Original message: %s\n", str);
    printf("Encrypted message: ");
    for(int i = 0; i < strlen(str); i++) {
        mpz_set_ui(m, str[i]);
        encrypt(c, m, e, n);
        gmp_printf("%Zd ", c);
    }
    printf("\n");
    gmp_printf("Entered p number: %Zd\n", p);
    gmp_printf("Entered q number: %Zd\n", q);
    gmp_printf("Number e: %Zd\n", e);
    gmp_printf("RSA parameter n: %Zd\n", n);
    gmp_printf("RSA parameter phi_n: %Zd\n", phi_n);
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
    mpz_clears(p_minus_1, q_minus_1, NULL);
    
    return 0;
}
