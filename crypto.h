#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* caesar_cypher1(int num, const char* input) {
    int length = strlen(input);
    char* output = malloc(length + 1);  // Allocate memory for the output string
    for(int i = 0; i < length; i++) {
        char ch = input[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            char base = (ch >= 'a' && ch <= 'z') ? 'a' : 'A';
            output[i] = ((ch - base + num) % 26) + base;
        } else {
            // If the character is not a letter, return the same character
            output[i] = ch;
        }
    }
    output[length] = '\0';  // Don't forget to null-terminate the output string
    return output;
}

int main() {
    int num = 1;
    const char message[] = "hola bon dia";
    char* encrypted = caesar_cypher1(num, message);
    printf("Mensaje original: %s\n", message);
    printf("Mensaje cifrado: %s\n", encrypted);
    free(encrypted);  // Don't forget to free the allocated memory
    return 0;
}


char affine_cypher(const char *alphabet, int a, int b, char ch) {
    if (ch == ' ') return ' '; // Ignore spaces
    int alphabetSize = strlen(alphabet);
    char *position = strchr(alphabet, ch);
    if (position != NULL) {
        int charIndex = position - alphabet;
        // Calculate the new character position after adding num
        char newIndex = (charIndex*a + b) % alphabetSize;
        if (newIndex < 0) newIndex += alphabetSize; // Handle negative shifts
        return alphabet[newIndex];
    } else {
        // If the character is not found in the alphabet, return the same character
        return ch;
    }
}
int main() {
    const char alphabet[] = "ABCCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char message[] = "TRENQUEU ELS VIDRES";
    int a = 8, b = 12;
    for (int i = 0; i < sizeof(message) - 1; i++) {
      printf("%c",affine_cypher(alphabet, a, b, message[i]));
    }
    printf("\n");
    return 0;
}

// Function to calculate the greatest common divisor (GCD) using Euclidean Algorithm
int gcd(int a, int b, int *x, int *y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    int d = gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return d;
}

// Function to find the modular inverse of 'a' mod 'm'
int modular_inverse(int a, int m) {
    int x, y;
    int g = gcd(a, m, &x, &y);
    if (g != 1) {
        // Modular inverse doesn't exist
        return -1;
    } else {
        // Ensure the result is positive
        return (x % m + m) % m;
    }
}

char affine_decypher(const char *alphabet, const int alphabetSize, int inverse, int b, char ch) {
    if (inverse == -1) return '#';
    if (ch == ' ') return ' '; // Ignore spaces
    char *position = strchr(alphabet, ch);
    if (position != NULL) {
        int charIndex = position - alphabet;
        char newIndex = ((charIndex-b)*inverse) % alphabetSize;
        if (newIndex < 0) newIndex += alphabetSize; // Handle negative shifts
        return alphabet[newIndex];
    } else {// If the character is not found in the alphabet, return the same character
        return '#';
    }
}

int main() {
    const char alphabet[] = "ABCCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int alphabetSize = strlen(alphabet);
    const char message[] = "JUYPMRYR YAC ZCQUYC";
    printf("%s\n",message);
    const int a = 8, b = 12;
    const int inverse = modular_inverse(a, alphabetSize);
    for (int i = 0; i < sizeof(message) - 1; i++) {
      printf("%c",affine_decypher(alphabet, alphabetSize, inverse, b, message[i]));
    }
    printf("\n");
    return 0;
}
