#ifndef CRYPTO_UTIL
#define CRYPTO_UTIL

#include <ctime>
#include <crypto++/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <bits/stdc++.h>

using namespace CryptoPP;
using namespace std;

#define AES128_KEY_LEN 16

// hash function with salt
string H1(string message, string key);

// hash function without salt
string H(string message);

// hash function: return int
int hash_k_int(string message, string key);

// byte to Int
int bytesToInt(byte *bytes, int size);

// Int to byte
byte *IntToBytes(int num);

// padding with zero
string padding(string s, int len);

// xor
string Xor(string s1, string s2);

// random key: return string
string Gen_RandKey(int len);

// AES-CFB encrypt
void encrypt(string key, string plaintext, string &ciphertext);

// AES-CFB decrypt
void decrypt(string key, string ciphertext, string &plaintext);

#endif //CRYPTO_UTIL
