# Enigma cryptographic calculator
A calculator providing support for basic mathematical operations and cryptographic functions written in C.

<a href="url"><img src="https://github.com/jordii222/CalculadoraRSA/assets/157251726/e56a6851-4070-4717-9501-5f033aba9385" width=75% height=75%></a>

## Features
**Basic mathematics:**

Basic mathematic expressions with several operations and operands are supported (+,-,*,/).

**Supported cryptographic functions:**

* Caesar cypher
* Afine cypher
* RSA

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

## General structure
```
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
## Cryptographic algorithms
```
  ┌───────────────────────────────────────────┐      ┌────────────────────────────────────────────────┐      ┌─────────────────────────────────────────────────┐
  │ RSA algorithm                             │      │ Diffie-Hellman key exchange algorithm          │      │ ElGamal encryption algorithm                    │
  │                                           │      │                                                │      │                                                 │
  │ Bob (receiver):                           │      │ Alice and Bob:                                 │      │ Alice and Bob:                                  │
  │ 1. Chose two big primers p and q          │      │ 1. Publicly agree on a large prime mod p       │      │ 1. Publicly agree on a large prime mod p        │
  │ 2. Compute n=p*q and ϕ(n)=(p-1)*(q-1)     │      │    and a primitive root g mod p                │      │    and a primitive root g mod p                 │
  │ 3. Chose an exponent e (hardcoded?)       │      │ 2. Chose a random number n between 1 and p−1   │      │                                                 │
  │    with 2<e<ϕ(n)and coprime with ϕ(n)     │      │    and compute g^n                             │      │ Bob:(receiver):                                 │
  │ 4. Compute the inverse d of e modulo ϕ(n) │      │    a and g^a for Alice and b and g^b for Bob   │      │ 2. Choses a random number b between 1 and p−1   │
  │    (extended Euclid's algotithm)          │      │    Public key: g^n                             │      │    and compute g^b                              │
  │    d*e≡1 mod ϕ(n)                         │      │    Private key: n                              │      │                                                 │
  │ Public key: (n,e)                         │      │ 3. Both share their respective public keys     │      │ Alicer(sender):                                 │
  │ Private key: d                            │      │ 4. Both compute the shared key g^a^b and g^b^a │      │ 3. Choses a efimeral number a between 1 and p−1 │
  │                                           │      │    K=g^a^b                                     │      │    and compute g^a                              │
  │ Alice (sender):                           │      └────────────────────────────────────────────────┘      │ 4. Computes the shared secret K=(g^b)^a         │
  │ 5. Encode message (m) into blocs          │                                                              │ 5. Encrypts the message using c≡m*K mod p       │
  │ 6. Encrypt blocs using c≡m^e mod n        │                                                              │    and sends it together with g^a               │
  │    with Bob's public key                  │                                                              │                                                 │
  │ 7. Send c                                 │                                                              │ Bob (receiver):                                 │
  │                                           │                                                              │ 6. Computes the shared secret K=(g^b)^a         │
  │ Bob (receiver):                           │                                                              │ 7. Decrypts the message using m≡c*K^-1≡m mod p  │
  │ 8. Decrypt using c^d≡m                    │                                                              └─────────────────────────────────────────────────┘
  └───────────────────────────────────────────┘
```
## Arbitrary precision computations
```
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
