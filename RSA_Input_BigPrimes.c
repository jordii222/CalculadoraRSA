#include <stdio.h>
#include <gmp.h>
#include <string.h>

int is_prime(mpz_t num) {
    return mpz_probab_prime_p(num, 25);
}

void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d) {
    // Calcular n = p * q
    mpz_mul(n, p, q);

    // Calcular phi_n = (p-1) * (q-1)
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi_n, p, q);

    // Elegir e tal que 1 < e < phi_n y gcd(e, phi_n) = 1
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

    // Calcular d = e^-1 mod phi_n
    if(mpz_invert(d, e, phi_n) == 0) {
        mpz_clear(phi_n);
        gmp_printf("Inverso modular no existe\n");
        return;
    }
}

void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    // Calcular c = m^e mod n
    mpz_powm(c, m, e, n);
}

void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    // Calcular m = c^d mod n
    mpz_powm(m, c, d, n);
}

int main(void) {
    mpz_t p, q, n, phi_n, e, d, m, c;
    char p_str[1024], q_str[1024];

    mpz_inits(p, q, n, phi_n, e, d, m, c, NULL);

    // Solicitar al usuario los números primos
    printf("Introduce el número primo p: ");
    scanf("%1023s", p_str);
    mpz_set_str(p, p_str, 10);
    if(!is_prime(p)) {
        printf("p no es un número primo.\n");
        return 1;
    }

    printf("Introduce el número primo q: ");
    scanf("%1023s", q_str);
    mpz_set_str(q, q_str, 10);
    if(!is_prime(q)) {
        printf("q no es un número primo.\n");
        return 1;
    }

    // Calcular los parámetros de RSA
    calculate_rsa_params(p, q, n, phi_n, e, d);

    // Cifrar y descifrar cada carácter
    char str[] = "holaa";
    printf("Mensaje original: %s\n", str);
    printf("Mensaje cifrado: ");
    for(int i = 0; i < strlen(str); i++) {
        mpz_set_ui(m, str[i]);
        encrypt(c, m, e, n);
        gmp_printf("%Zd ", c);
    }
    printf("\n");
    gmp_printf("Número primo p introducido: %Zd\n", p);
    gmp_printf("Número primo q introducido: %Zd\n", q);
    gmp_printf("Parámetro n de RSA: %Zd\n", n);
    gmp_printf("Parámetro phi_n de RSA: %Zd\n", phi_n);
    gmp_printf("Parámetro d de RSA: %Zd\n", d); 
    printf("\nMensaje descifrado: ");
    for(int i = 0; i < strlen(str); i++) {
        mpz_set_ui(m, str[i]);
        encrypt(c, m, e, n);
        decrypt(m, c, d, n);
        printf("%ld", mpz_get_ui(m));
        printf("(%c)", (char)mpz_get_ui(m));

    }
    printf("\n");

    mpz_clears(p, q, n, phi_n, e, d, m, c, NULL);

    return 0;
}
