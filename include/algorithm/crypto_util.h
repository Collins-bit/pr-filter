#ifndef CRYPTO_UTIL
#define CRYPTO_UTIL

#include <sys/time.h>
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

// hash函数 带盐值
std::string H1(const std::string message, std::string key);

// hash函数，不带盐值
std::string H(const std::string message);

// hash函数，输出Int
int hash_k_int(const std::string message, const std::string key);

// byte转Int
int bytesToInt(byte *bytes, int size);

// Int转byte
byte *IntToBytes(int num);

// 整数转二进制
std::string toBinary(int n);

// 填充算法: 后面填0
std::string padding(std::string s, int len);

// string 异或
std::string Xor(std::string s1, std::string s2);

// 产生随机数密钥串，类型为byte
int gen_key(byte *key1);

// 产生随机数密钥串，类型为string
std::string Gen_RandKey(int len);

// AES加密
void encrypt(std::string key, std::string plaintext,std::string &ciphertext);

// AES解密
void decrypt(std::string key, std::string ciphertext, std::string &plaintext);

#endif //CRYPTO_UTIL
