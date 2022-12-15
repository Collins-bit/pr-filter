#include <algorithm/crypto_util.h>
using namespace CryptoPP;

// hash函数 带盐值
std::string H1(const std::string message, std::string key)
{
    byte buf[SHA256::DIGESTSIZE];
    std::string salt = key;
    SHA256().CalculateDigest(buf, (byte *)((message + salt).c_str()), message.length() + salt.length());
    return std::string((const char *)buf, (size_t)SHA256::DIGESTSIZE);
}

// hash函数，不带盐值
std::string H(const std::string message)
{
    byte buf[SHA256::DIGESTSIZE];
    std::string salt = "01";
    SHA256().CalculateDigest(buf, (byte *)((message + salt).c_str()), message.length() + salt.length());
    return std::string((const char *)buf, (size_t)SHA256::DIGESTSIZE);
}

// hash函数，输出Int
int hash_k_int(const std::string message, const std::string key)
{
    byte buf[SHA256::DIGESTSIZE];
    std::string salt = key;
    SHA256().CalculateDigest(buf, (byte *)((message + salt).c_str()), message.length() + salt.length());
    return bytesToInt(buf, 4);
}

// byte转Int
int bytesToInt(byte *bytes, int size = 4)
{
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

// Int转byte
byte *IntToBytes(int num)
{
    byte *ans = new byte[4];
    ans[0] = (byte)(num);
    ans[1] = (byte)(num >> 8);
    ans[2] = (byte)(num >> 16);
    ans[3] = (byte)(num >> 24);
    return ans;
}

// 整数转二进制
std::string toBinary(int n)
{
    std::string r;
    while (n != 0)
    {
        r += (n % 2 == 0 ? "0" : "1");
        n /= 2;
    }
    reverse(r.begin(), r.end());
    return r;
}

// 填充算法: 后面填0
std::string padding(std::string s, int len)
{
    std::string r;
    if (s.size() < len)
    {
        int count = len - s.size();
        std::string pad(count, '0');
        r = s + pad;
    }
    else
    {
        r = s.substr(0, len);
    }
    return r;
}

// string 异或
std::string Xor(std::string s1, std::string s2)
{
    if (s1.length() != s2.length())
    {
        std::cout << "[Warning] [Xor] not sufficient size, s1 lenght: " << s1.length() << ", s2 lenght: " << s2.length() << std::endl;
    }
    if (s1.length() < s2.length())
    {
        std::string tmp = s1;
        s1 = s2;
        s2 = tmp;
    }
    std::string ans = s1;
    for (int i = 0; i < s2.length(); i++)
    {
        ans[i] = ans[i] ^ s2[i];
    }
    return ans;
}

// 产生随机数密钥串，类型为byte
int gen_key(byte *key1)
{
    // 产生一个随机数密钥串，长度为16字节
    AutoSeededRandomPool rand;
    SecByteBlock Key(0x00, AES::DEFAULT_KEYLENGTH);
    rand.GenerateBlock(Key, Key.size());
    key1 = Key;
    return 1;
}

// 产生随机数密钥串，类型为string
std::string Gen_RandKey(int len)
{
    std::string key = "";
    int rn;
    while (len != 0)
    {
        rn = rand();
        while (rn != 0)
        {
            if (len == 0)
                break;
            key += std::to_string((rn % 10) % 2);
            rn /= 10;
            --len;
        }
    }
    return key;
}

// AES加密函数
void encrypt(std::string key, std::string plaintext, std::string &ciphertext)
{
    try
    {
        key = padding(key, 16);
        byte iv_s[17] = "0123456789abcdef";
        CFB_Mode<AES>::Encryption e;
        e.SetKeyWithIV((byte *)key.c_str(), AES128_KEY_LEN, iv_s, (size_t)AES::BLOCKSIZE);
        byte tmp_new_st[plaintext.length()] = {0};
        e.ProcessData(tmp_new_st, (byte *)plaintext.c_str(), plaintext.length());
        ciphertext = std::string((const char *)tmp_new_st, plaintext.length());
    }
    catch (const CryptoPP::Exception &e)
    {
        std::cerr << "encrypt wrong: " << e.what() << std::endl;
        exit(1);
    }
}

// AES解密函数
void decrypt(std::string key, std::string ciphertext, std::string &plaintext)
{
    try
    {
        key = padding(key, 16);
        byte iv_s[17] = "0123456789abcdef";
        CFB_Mode<AES>::Decryption d;
        d.SetKeyWithIV((byte *)key.c_str(), AES128_KEY_LEN, iv_s, (size_t)AES::BLOCKSIZE);
        byte tmp_new_st[ciphertext.length()] = {0};
        d.ProcessData(tmp_new_st, (byte *)ciphertext.c_str(), ciphertext.length());
        plaintext = std::string((const char *)tmp_new_st, ciphertext.length());
    }
    catch (const CryptoPP::Exception &e)
    {
        std::cerr << "decrypt wrong: " << e.what() << std::endl;
        exit(1);
    }
}