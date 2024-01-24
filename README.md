# Enigma cryptographic calculator
A calculator providing support for basic mathematical operations and cryptographic functions written in C.

<a href="url"><img src="https://github.com/jordii222/CalculadoraRSA/assets/157251726/e56a6851-4070-4717-9501-5f033aba9385" width=75% height=75%></a>

## Features
**Basic mathematics:**

Basic mathematic expressions with several operations and operands are supported (+,-,*,/).

For example $3 * \sin(1.5708) + 4 / 2$ is a valid expression.

The following trigonometrical functions exist:
```
  >> sin(expression)
  >> cos(expression)
```
Other redundant textual functions can be used:
```
  >> add(expression1,expression2,expression3,...)
  >> multiply(expression1,expression2,expression3,...)
```
where _expressions_ are numbers or numerically evaluated expressions.

**Cryptographic functions:**

* Caesar cypher
```
  >> caesar(shift, "message")
```
where shift is the displacement value.
* Afine cypher
```
  >> affine(a, b, "message")
```
where _a_ and _b_ are the coefficients ($c=a\cdot m + b\ mod\ \textrm{alphabet size}$).
* RSA
```
  >> rsa("prime1","prime2","message")
```
where _p_ is _prime1_ and _q_ is _prime2_.

**Other functions:**

* Prime generation
```
  >> bigprime(bits)
```
where _bits_ is the length in bits of the generated prime.
* 
**User functions:**

* Clear screen
```
  >> cl()
```
* Exit
```
  >> exit()
  >> quit()
  >> q()
```

## Operative structure
```
 PROGRAM CONCEPT:


  ┌─────────────────────┐
  │  CLI Interface:     │
  │                     │
  │  STDIN & STDOUT     │ Program  ◄────────────────────────────────────────────────────────────────┐
  │  Interactive mode   │ output                                                                    │
  └─────────────────────┘                                                                           │
                                                                                                    │
       User input                                                                                   │
                                                                                                    │
           │                                                                                        │
           │                                                                                        │
           │                                                                                        │
           │                                                                                        │
           ▼                                                                                        │
                                                                                                    │
    Parsing algorithm                                                                               │
    │                                                           ┌─────────┐                         │
    └──►  Abstract syntax tree (AST)                            │ Symbols │                         │
                                                                └─────────┘                         │
              │                                                                                     │
              │                                             ┌───► Numbers   ────┐                   │
              │                                             │                   │                   │
              ▼                                             │                   │                   │
                                                            │                   │
          Tokenization  ──────────►  Tree building  ────────┼───► Operators ────┼──────►  Expression evaluation
       (symbol extraction)        (operation priority)      │                   │               (Result)
                                                            │                   │
                                                            │                   │
       ┌───────────────┐                                    └───► Functions ────┘
       │               │
       │  AST example  │                                              │
       │               │                                              │
       │       +       │                                              │
       │      / \      │                                              ▼
       │     *   /     │
       │    / \ / \    │                                    Basic mathematical operations (+,-,*,/)
       │   3 sin 4 2   │
       │      |        │                                    +
       │    1.5708     │
       │               │                                    Functions
       └───────────────┘                                    │
                                                            ├─►  Caesar cryptosystem (encrypt/decrypt)
                                                            │
                                                            ├─►  Afine cryptosystems (encrypt/decrypt)
                                                            │
                                                            └─►  RSA (encrypt/decrypt)
```
## File and function wise structure
```
 PROGRAM FLOW:


          enigma.c  ──────────────►  ast.h  ────────────────►  crypto.h

             │                         │                          │
             │                         │                          │
             ▼                         ▼                          ▼
       ┌──────────────┐       ┌─────────────────┐         ┌───────────────┐
       │ Main program │       │ Abstract Syntax │         │ Cryptographic │
       └──────────────┘       │   Tree parser   │         │    library    │
       Handles arguments      └─────────────────┘         └───────────────┘
       Receives input         Parses input text           Implements the main
       Passes input to the    into numbers, text,         cyptographic functions
       AST parser.            operators and functions.    such as caesar(), affine()
                              Evaluates expressions       and rsa() as well
                              calling the more advanced
                              functions in crypto.h when
                              needed.
```
```
FUNCTION DESCRIPTION AND CALLS:


┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│                                                                                                                                                               │
│ ┌──────────┐                                                                                                                                                  │
│ │ enigma.c │                                                                                                                                                  │
│ └──────────┘                                                                                                                                                  │
│                                                                                                                                                               │
│   - int actio():                                                                                                                                              │
│     + Description: Executes interactive mode, parsing and evaluating user input.                                                                              │
│     + Calls: parseExpr(), eval()                                                                                                                              │
│                                                                                                                                                               │
│   - int lectio(int argc, char argv[]):                                                                                                                        │
│     + Description: Reads expressions from command arguments, concatenates them, and outputs the result.                                                       │
│     + Calls: parseExpr(), eval()                                                                                                                              │
│                                                                                                                                                               │
│   - void help():                                                                                                                                              │
│     + Description: Prints a help message with usage information.                                                                                              │
│                                                                                                                                                               │
│   - void title():                                                                                                                                             │
│     + Description: Prints the title of the program.                                                                                                           │
│                                                                                                                                                               │
│   - int main(int argc, char argv[]):                                                                                                                          │
│     + Description: Main entry point of the program. Handles command-line arguments and executes the appropriate functionality.                                │
│     + Calls: title(), actio(), lectio(), help()                                                                                                               │
│                                                                                                                                                               │
│ ┌───────┐                                                                                                                                                     │
│ │ ast.h │                                                                                                                                                     │
│ └───────┘                                                                                                                                                     │
│                                                                                                                                                               │
│   - AST newNode(char op, double num, char name, AST args, int numArgs, AST left, AST right):                                                                  │
│     + Description: Creates a new node for the Abstract Syntax Tree (AST).                                                                                     │
│                                                                                                                                                               │
│   - double parseNum(char s):                                                                                                                                  │
│     + Description: Parses a number from the input string and moves the string forward.                                                                        │
│                                                                                                                                                               │
│   - AST parseExpr(char s):                                                                                                                                    │
│     + Description: Parses an expression and moves the string forward.                                                                                         │
│     + Calls: parseTerm()                                                                                                                                      │
│                                                                                                                                                               │
│   - AST parseFactor(char s):                                                                                                                                  │
│     + Description: Parses a factor and moves the string forward.                                                                                              │
│     + Calls: parseExpr()                                                                                                                                      │
│                                                                                                                                                               │
│   - AST parseFunction(char s):                                                                                                                                │
│     + Description: Parses a function with arguments and moves the string forward.                                                                             │
│     + Calls: parseExpr()                                                                                                                                      │
│                                                                                                                                                               │
│   - AST parseTerm(char s):                                                                                                                                    │
│     + Description: Parses a term and moves the string forward.                                                                                                │
│     + Calls: parseExpr(), parseFunction()                                                                                                                     │
│                                                                                                                                                               │
│   - double eval(AST node):                                                                                                                                    │
│     + Description: Evaluates the AST and returns the result.                                                                                                  │
│                                                                                                                                                               │
│ ┌──────────┐                                                                                                                                                  │
│ │ crypto.h │                                                                                                                                                  │
│ └──────────┘                                                                                                                                                  │
│                                                                                                                                                               │
│   - char caesar(int n, const char s):                                                                                                                         │
│     + Description: Applies the Caesar cipher to a string.                                                                                                     │
│                                                                                                                                                               │
│   - char affine(int a, int b, const char s):                                                                                                                  │
│     + Description: Applies the Affine cipher to a string.                                                                                                     │
│                                                                                                                                                               │
│   - int is_prime(mpz_t num):                                                                                                                                  │
│     + Description: Checks if a given number is prime.                                                                                                         │
│     + Calls: mpz_probab_prime_p()                                                                                                                             │
│                                                                                                                                                               │
│   - void calculate_rsa_params(mpz_t p, mpz_t q, mpz_t n, mpz_t phi_n, mpz_t e, mpz_t d):                                                                      │
│     + Description: Calculates parameters for RSA encryption.                                                                                                  │
│     + Calls: mpz_mul(), mpz_sub_ui(), mpz_gcd(), mpz_invert()                                                                                                 │
│                                                                                                                                                               │
│   - void encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n):                                                                                                         │
│     + Description: Encrypts a message using RSA.                                                                                                              │
│     + Calls: mpz_powm()                                                                                                                                       │
│                                                                                                                                                               │
│   - void decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n):                                                                                                         │
│     + Description: Decrypts a message using RSA.                                                                                                              │
│     + Calls: mpz_powm()                                                                                                                                       │
│                                                                                                                                                               │
│   - void rsa(const char prime_str1, const char prime_str2, const char message):                                                                               │
│     + Description: Performs RSA encryption and decryption on a message.                                                                                       │
│     + Calls: mpz_set_str(), is_prime(), calculate_rsa_params(), encrypt(), decrypt(), mpz_get_ui()                                                            │
│                                                                                                                                                               │
│   - char big_prime(int bits):                                                                                                                                 │
│     + Description: Generates a big random prime of a certain bit length.                                                                                      │
│     + Calls: mpz_init(), gmp_randinit_default(), gmp_randseed_ui(), mpz_urandomb(), mpz_setbit(), mpz_nextprime(), mpz_clear(), gmp_randclear(), mpz_get_str()│
└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
```
## Cryptographic algorithms
This is a summary of several cryptographic or cryptography related algorithms. This program only implements Caesar's cryptosystem, affine cryptosystem and RSA cryptosystem.
```
 STUDIED ALGORITHMS:


 ┌───────────────────────────────────────────────────────┐                ┌───────────────────────────────────────────┐
 │ Caesar Cipher:                                        │                │ RSA algorithm                             │
 │                                                       │                │                                           │
 │ Alice (sender):                                       │                │ Bob (receiver):                           │
 │ 1. Choose a shift key K                               │                │ 1. Chose two big primers p and q          │
 │ 2. Encrypt the message m shifting every character     │                │ 2. Compute n=p*q and ϕ(n)=(p-1)*(q-1)     │
 │    by K positions in the alphabet                     │                │ 3. Chose an exponent e (hardcoded?)       │
 │ 4. Send the message                                   │                │    with 2<e<ϕ(n)and coprime with ϕ(n)     │
 │                                                       │                │ 4. Compute the inverse d of e modulo ϕ(n) │
 │ Bob (receiver):                                       │                │    (extended Euclid's algotithm)          │
 │ 5. Shift every letter by -K positions in the alphabet │                │    d*e≡1 mod ϕ(n)                         │
 │    to recover the original message m                  │                │ Public key: (n,e)                         │
 └───────────────────────────────────────────────────────┘                │ Private key: d                            │
                                                                          │                                           │
 ┌────────────────────────────────────────────────────────────────┐       │ Alice (sender):                           │
 │ Affine Cipher:                                                 │       │ 5. Encode message (m) into blocs          │
 │                                                                │       │ 6. Encrypt blocs using c≡m^e mod n        │
 │ Alice:(sender):                                                │       │    with Bob's public key                  │
 │ 1. Choose two keys a and b such that a                         │       │ 7. Send c                                 │
 │    and the size of the alphabet are coprime                    │       │                                           │
 │ 2. Encode the message                                          │       │ Bob (receiver):                           │
 │ 3. Compute (a*x + b) mod the alphabet size for every character │       │ 8. Decrypt using c^d≡m                    │
 │ 4. Sends the message                                           │       └───────────────────────────────────────────┘
 │                                                                │
 │ Bob (receiver):                                                │ ┌─────────────────────────────────────────────────┐
 │ 5. Compute a_inv * (x - b) mod the alphabet size for each      │ │ ElGamal encryption algorithm                    │
 │    character where a_inv is the multiplicative inverse of      │ │                                                 │
 │    a modulo the alphabet size                                  │ │ Alice and Bob:                                  │
 └────────────────────────────────────────────────────────────────┘ │ 1. Publicly agree on a large prime mod p        │
                                                                    │    and a primitive root g mod p                 │
                                                                    │                                                 │
 ┌────────────────────────────────────────────────┐                 │ Bob:(receiver):                                 │
 │ Diffie-Hellman key exchange algorithm          │                 │ 2. Choses a random number b between 1 and p−1   │
 │                                                │                 │    and compute g^b                              │
 │ Alice and Bob:                                 │                 │                                                 │
 │ 1. Publicly agree on a large prime mod p       │                 │ Alicer(sender):                                 │
 │    and a primitive root g mod p                │                 │ 3. Choses a efimeral number a between 1 and p−1 │
 │ 2. Chose a random number n between 1 and p−1   │                 │    and compute g^a                              │
 │    and compute g^n                             │                 │ 4. Computes the shared secret K=(g^b)^a         │
 │    a and g^a for Alice and b and g^b for Bob   │                 │ 5. Encrypts the message using c≡m*K mod p       │
 │    Public key: g^n                             │                 │    and sends it together with g^a               │
 │    Private key: n                              │                 │                                                 │
 │ 3. Both share their respective public keys     │                 │ Bob (receiver):                                 │
 │ 4. Both compute the shared key g^a^b and g^b^a │                 │ 6. Computes the shared secret K=(g^b)^a         │
 │    K=g^a^b                                     │                 │ 7. Decrypts the message using m≡c*K^-1≡m mod p  │
 └────────────────────────────────────────────────┘                 └─────────────────────────────────────────────────┘
```
## Arbitrary precision computations
It was initially planned to implement our own library for handling arbitrary precision calculations but ultimately gmp has been used for simplicity.
```
 ARBITRARY PRECISION:


      Operations with really big integers

                   │
                   │
                   ▼

        Arbitrary precision library  ─────────►  External library
                                                     (gmp.h)
                   │
                   │
                   │
                   ▼
      ┌──────────────────────────────┐
      │ Required operations:         │
      │                              │
      │ Addition/subtraction         │
      │ Multiplication               │
      │ Exponentiation               │
      │ Integer division and modulus │
      └──────────────────────────────┘
```

## Useful development resources
* ASCIIFlow for brainstorming and program flow control (recommended for schema.txt visualization)

  https://asciiflow.com/

* ChatGPT for base code generation

  https://chat.openai.com/auth/login

* Copilot for base code generation

  https://copilot.microsoft.com/

* Wolfram Alpha for big prime generation

  https://www.wolframalpha.com/

* Online C compilers and debbugers
  
  https://www.onlinegdb.com/online_c_compiler
  
  https://www.programiz.com/c-programming/online-compiler/

* RSA examples for testing purposes

  https://condor.depaul.edu/ichu/csc415/notes/notes4/rsa.html
