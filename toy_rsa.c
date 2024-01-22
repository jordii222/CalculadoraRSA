#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
const long long LLONG_MAX = 9223372036854775807; /* Instead of #include <limits.h> */

static long long fast_modulo_exponentiation(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}
static int primality(long long n, int k) {
    if (n <= 1 || n <= 3) return n == 2 || n == 3; /* check */
    if (n % 2 == 0) return 0;
    long long r, d;
    r = n - 1;
    d = 0;
    while (r % 2 == 0) {
        r /= 2;
        d++;
    }
    for (int i = 0; i < k; i++) {
        long long a = 2 + rand() % (n - 3);
        long long x = fast_modulo_exponentiation(a, r, n);
        if (x == 1 || x == n - 1) continue;
        for (long long j = 0; j < d - 1; j++) {
            x = fast_modulo_exponentiation(x, 2, n);
            if (x == n - 1)
                break;
        }
        if (x != n - 1) return 0;
    }
    return 1;
}

long long big_prime() {
    long long max_value = sqrt((double)LLONG_MAX);
    long long prime;
    while (1) {
        prime = (rand() % (max_value - 2)) + 2;
        if (primality(prime, 10)) return prime;
    }
}

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}
long long coprime(long long n) {
    long long e = 3; /* Start with a small value for 'e'. */
    while (1) {
        if (gcd(e, n) == 1) return e; /* Found a coprime 'e' */
        e++;
    }
}

long long mod_inverse(long long e, long long phi_n) {
    long long a = e;
    long long b = phi_n;
    long long x0 = 1, x1 = 0;
    while (b > 0) {
        long long q = a / b;
        long long temp = x1;
        x1 = x0 - q * x1;
        x0 = temp;
        temp = b;
        b = a - q * b;
        a = temp;
    }
    if (x0 < 0) x0 += phi_n;
    return x0;
}

typedef struct  {
  const long long n;
  const long long e;
  const long long d;
} KeyPair;

KeyPair pub_priv_key() {
  const long long p = big_prime();
  long long q = big_prime();
  while(q == p) {
    q = big_prime();
    printf("Finding more primes\n");
  }
  const long long n = p * q;
  const long long phi_n = (p-1)*(q-1);
  const long long e = coprime(n);
  const long long d = mod_inverse(e, phi_n);
  printf("p: %lld\nq: %lld\nn: %lld\nphi(n): %lld\ne: %lld\nd: %lld\n", p, q, n, phi_n, e, d);
  KeyPair keyp = {n, e, d};
  return keyp;
}

long long block_size(const char *alphabet, long long n) {
    int alphabet_length = strlen(alphabet);
    long long size = ceil(log(n) / log(alphabet_length));
    return size;
}

/* Function to find the index of a character in the alphabet */
int get_char_index(const char *alphabet, char c) {
    const char *ptr = strchr(alphabet, c);
    if (ptr != NULL) {
        return ptr - alphabet;
    } else {
        /* Character not found in the alphabet; you can handle this case as needed. */
        /* For simplicity, we assume it's 0. */
        return 0;
    }
}

/* Function to encode a message into blocks */
long long *encode_message(const char *alphabet, const char *message, int block_size) {
    int alphabet_length = strlen(alphabet);
    int message_length = strlen(message);
    /* Calculate the number of blocks needed */
    int num_blocks = (message_length + block_size - 1) / block_size;
    /* Allocate memory for the array of blocks */
    long long *blocks = (long long *)malloc(num_blocks * sizeof(long long));
    for (int i = 0; i < num_blocks; i++) {
        /* Initialize the block */
        blocks[i] = 0;
        /* Encode characters into the block */
        for (int j = 0; j < block_size; j++) {
            int index = i * block_size + j;
            if (index < message_length) {
                int char_index = get_char_index(alphabet, message[index]);
                blocks[i] = (blocks[i] * alphabet_length) + char_index;
            } else {
                /* If the block is partially empty, fill it with a character from the alphabet */
                blocks[i] = (blocks[i] * alphabet_length) + get_char_index(alphabet, alphabet[0]);
            }
        }
    }
    return blocks;
}

long long *encrypt_message(const long long *blocks, int num_blocks, long long e, long long n) {
    long long *encrypted_blocks = (long long *)malloc(num_blocks * sizeof(long long));
    for (int i = 0; i < num_blocks; i++) {
        encrypted_blocks[i] = fast_modulo_exponentiation(blocks[i], e, n);
    }
    return encrypted_blocks;
}

long long *decrypt_message(const long long *encrypted_blocks, int num_blocks, long long d, long long n) {
    long long *decrypted_blocks = (long long *)malloc(num_blocks * sizeof(long long));
    for (int i = 0; i < num_blocks; i++) {
        decrypted_blocks[i] = fast_modulo_exponentiation(encrypted_blocks[i], d, n);
    }
    return decrypted_blocks;
}

/* Function to decode blocks into the original message */
char *decode_message(const char *alphabet, const long long *blocks, int num_blocks, int block_size) {
    int alphabet_length = strlen(alphabet);
    int message_length = num_blocks * block_size;
    char *decoded_message = (char *)malloc((message_length + 1) * sizeof(char));

    for (int i = 0; i < num_blocks; i++) {
        long long block = blocks[i];
        for (int j = block_size - 1; j >= 0; j--) {
            int char_index = block % alphabet_length;
            decoded_message[i * block_size + j] = alphabet[char_index];
            block /= alphabet_length;
        }
    }

    decoded_message[message_length] = '\0'; /* Null-terminate the string */
    return decoded_message;
}


int main() {
  srand(time(NULL)); /* Initialize random seed */

  /* Create public and private keys */
  KeyPair keyp = pub_priv_key();
  printf("Public key:\n\tn = %lld\n\te = %lld\nPrivate key:\n\td = %lld\n", keyp.n, keyp.e, keyp.d);

  /* Define and encode message into blocks */
  const char* alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,!?\n";
  const char* message = "hola bona tarda";
  /* Print the message */
  printf("Message:\n%s\n", message);

  const long long bs = block_size(alphabet, keyp.n);
  long long *encoded_blocks = encode_message(alphabet, message, bs);

  /* Print the encoded blocks */
  printf("Encoded blocks:\n");
  int num_blocks = (strlen(message) + bs - 1) / bs;
  for (int i = 0; i < num_blocks; i++) printf("| %lld |", encoded_blocks[i]);
  printf("\n");

  /* Encrypt the mesage */
  long long *encrypt_blocks = encrypt_message(encoded_blocks, num_blocks, keyp.e, keyp.n);
  /* Print the encrypted blocks */
  printf("Encrypted blocks:\n");
  for (int i = 0; i < num_blocks; i++) printf("| %lld |", encrypt_blocks[i]);
  printf("\n");

  /* Decrypt the message */
  long long *decrypted_blocks = decrypt_message(encrypt_blocks, num_blocks, keyp.d, keyp.n);
  /* Print the decrypted blocks */
  printf("Decrypted blocks:\n");
  for (int i = 0; i < num_blocks; i++) printf("| %lld |", decrypted_blocks[i]);
  printf("\n");

  /* Decode the blocks into the original message */
  char *decoded_message = decode_message(alphabet, decrypted_blocks, num_blocks, bs);
  /* Print the decoded message */
  printf("Decoded message:\n%s\n", decoded_message);

  /* Free memory */
  free(encoded_blocks);
  free(encrypt_blocks);
  free(decrypted_blocks);
  free(decoded_message);

  return 0;
}

