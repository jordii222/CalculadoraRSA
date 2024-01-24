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
where _a_ and _b_ are the coefficients ($c=a\cdot m + b \textrm{ mod }\ alphabet_size$).
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
  >> exit
  >> quit
  >> q
```

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
This is a summary of several cryptographic or cryptography related algorithms. This program only implements Caesar's cryptosystem, affine cryptosystem and RSA cryptosystem.
```
┌───────────────────────────────────────────────────────┐               ┌───────────────────────────────────────────┐      
│ Caesar Cipher:                                        │               │ RSA algorithm                             │      
│                                                       │               │                                           │      
│ Alice (sender):                                       │               │ Bob (receiver):                           │      
│ 1. Choose a shift key K                               │               │ 1. Chose two big primers p and q          │      
│ 2. Encrypt the message m shifting every character     │               │ 2. Compute n=p*q and ϕ(n)=(p-1)*(q-1)     │      
│    by K positions in the alphabet                     │               │ 3. Chose an exponent e (hardcoded?)       │      
│ 4. Send the message                                   │               │    with 2<e<ϕ(n)and coprime with ϕ(n)     │      
│                                                       │               │ 4. Compute the inverse d of e modulo ϕ(n) │      
│ Bob (receiver):                                       │               │    (extended Euclid's algotithm)          │      
│ 5. Shift every letter by -K positions in the alphabet │               │    d*e≡1 mod ϕ(n)                         │      
│    to recover the original message m                  │               │ Public key: (n,e)                         │      
└───────────────────────────────────────────────────────┘               │ Private key: d                            │      
                                                                        │                                           │      
┌────────────────────────────────────────────────────────────────┐      │ Alice (sender):                           │      
│ Affine Cipher:                                                 │      │ 5. Encode message (m) into blocs          │      
│                                                                │      │ 6. Encrypt blocs using c≡m^e mod n        │      
│ Alice:(sender):                                                │      │    with Bob's public key                  │      
│ 1. Choose two keys a and b such that a                         │      │ 7. Send c                                 │      
│    and the size of the alphabet are coprime                    │      │                                           │      
│ 2. Encode the message                                          │      │ Bob (receiver):                           │      
│ 3. Compute (a*x + b) mod the alphabet size for every character │      │ 8. Decrypt using c^d≡m                    │      
│ 4. Sends the message                                           │      └───────────────────────────────────────────┘
│                                                                │
│ Bob (receiver):                                                │
│ 5. Compute a_inv * (x - b) mod the alphabet size for each      │
│    character where a_inv is the multiplicative inverse of      │
│    a modulo the alphabet size                                  │
└────────────────────────────────────────────────────────────────┘
┌────────────────────────────────────────────────┐      ┌─────────────────────────────────────────────────┐
│ Diffie-Hellman key exchange algorithm          │      │ ElGamal encryption algorithm                    │
│                                                │      │                                                 │
│ Alice and Bob:                                 │      │ Alice and Bob:                                  │
│ 1. Publicly agree on a large prime mod p       │      │ 1. Publicly agree on a large prime mod p        │
│    and a primitive root g mod p                │      │    and a primitive root g mod p                 │
│ 2. Chose a random number n between 1 and p−1   │      │                                                 │
│    and compute g^n                             │      │ Bob:(receiver):                                 │
│    a and g^a for Alice and b and g^b for Bob   │      │ 2. Choses a random number b between 1 and p−1   │
│    Public key: g^n                             │      │    and compute g^b                              │
│    Private key: n                              │      │                                                 │
│ 3. Both share their respective public keys     │      │ Alicer(sender):                                 │
│ 4. Both compute the shared key g^a^b and g^b^a │      │ 3. Choses a efimeral number a between 1 and p−1 │
│    K=g^a^b                                     │      │    and compute g^a                              │
└────────────────────────────────────────────────┘      │ 4. Computes the shared secret K=(g^b)^a         │
                                                        │ 5. Encrypts the message using c≡m*K mod p       │
                                                        │    and sends it together with g^a               │
                                                        │                                                 │
                                                        │ Bob (receiver):                                 │
                                                        │ 6. Computes the shared secret K=(g^b)^a         │
                                                        │ 7. Decrypts the message using m≡c*K^-1≡m mod p  │
                                                        └─────────────────────────────────────────────────┘

```
## Arbitrary precision computations
It was initially planned to implement our own library for handling arbitrary precision calculations but ultimately gmp has been used for simplicity.
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
