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

#define AES128_KEY_LEN 16

// hash function with salt
std::string H1(std::string message, std::string key);

// hash function without salt
std::string H(std::string message);

// hash function: return int
int hash_k_int(std::string message, std::string key);

// byte to Int
int bytesToInt(byte *bytes, int size);

// Int to byte
byte *IntToBytes(int num);

// padding with zero
std::string padding(std::string s, int len);

// xor
std::string Xor(std::string s1, std::string s2);

// random key: return string
std::string Gen_RandKey(int len);

// AES-CFB encrypt
void encrypt(std::string key, std::string plaintext, std::string &ciphertext);

// AES-CFB decrypt
void decrypt(std::string key, std::string ciphertext, std::string &plaintext);

#endif //CRYPTO_UTIL
