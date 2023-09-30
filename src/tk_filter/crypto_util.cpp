#include <tk_filter/crypto_util.h>

using namespace CryptoPP;

string H1(string message, string key) {
    byte buf[SHA256::DIGESTSIZE];
    string salt = key;
    SHA256().CalculateDigest(buf, (byte *) ((message + salt).c_str()), message.size() + salt.size());
    return string((const char *) buf, (size_t) SHA256::DIGESTSIZE);
}

string H(string message) {
    byte buf[SHA256::DIGESTSIZE];
    string salt = "01";
    SHA256().CalculateDigest(buf, (byte *) ((message + salt).c_str()), message.size() + salt.size());
    return string((const char *) buf, (size_t) SHA256::DIGESTSIZE);
}

int hash_k_int(string message, string key) {
    byte buf[SHA256::DIGESTSIZE];
    SHA256().CalculateDigest(buf, (byte *) ((message + key).c_str()), message.size() + key.size());
    return bytesToInt(buf, 4);
}

int bytesToInt(byte *bytes, int size = 4) {
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
}

byte *IntToBytes(int num) {
    byte *ans = new byte[4];
    ans[0] = (byte) (num);
    ans[1] = (byte) (num >> 8);
    ans[2] = (byte) (num >> 16);
    ans[3] = (byte) (num >> 24);
    return ans;
}

string padding(string s, int len) {
    string r;
    if (s.size() < len) {
        int count = len - s.size();
        string pad(count, '0');
        r = s + pad;
    } else {
        r = s.substr(0, len);
    }
    return r;
}

string Xor(string s1, string s2) {
//    if (s1.size() != s2.size()) {
//        cout << "[Warning] [Xor] not sufficient size, s1 lenght: " << s1.size() << ", s2 lenght: " << s2.size()
//                  << endl;
//    }
    if (s1.size() < s2.size()) {
        string tmp = s1;
        s1 = s2;
        s2 = tmp;
    }
    string ans = s1;
    for (int i = 0; i < s2.size(); i++) {
        ans[i] = ans[i] ^ s2[i];
    }
    return ans;
}

string Gen_RandKey(int len) {
    string key = "";
    int rn;
    while (len != 0) {
        rn = rand();
        while (rn != 0) {
            if (len == 0)
                break;
            key += to_string((rn % 10) % 2);
            rn /= 10;
            --len;
        }
    }
    return key;
}

void encrypt(string key, string plaintext, string &ciphertext) {
    try {
        key = padding(key, 16);
        byte iv_s[17] = "0123456789abcdef";
        CFB_Mode<AES>::Encryption e;
        e.SetKeyWithIV((byte *) key.c_str(), AES128_KEY_LEN, iv_s, (size_t) AES::BLOCKSIZE);
        byte tmp_new_st[plaintext.size()];
        e.ProcessData(tmp_new_st, (byte *) plaintext.c_str(), plaintext.size());
        ciphertext = string((const char *) tmp_new_st, plaintext.size());
    }
    catch (const CryptoPP::Exception &e) {
        cerr << "encrypt wrong: " << e.what() << endl;
        exit(1);
    }
}

void decrypt(string key, string ciphertext, string &plaintext) {
    try {
        key = padding(key, 16);
        byte iv_s[17] = "0123456789abcdef";
        CFB_Mode<AES>::Decryption d;
        d.SetKeyWithIV((byte *) key.c_str(), AES128_KEY_LEN, iv_s, (size_t) AES::BLOCKSIZE);
        byte tmp_new_st[ciphertext.size()];
        d.ProcessData(tmp_new_st, (byte *) ciphertext.c_str(), ciphertext.size());
        plaintext = string((const char *) tmp_new_st, ciphertext.size());
    }
    catch (const CryptoPP::Exception &e) {
        cerr << "decrypt wrong: " << e.what() << endl;
        exit(1);
    }
}