#ifndef PR_FILTER_UTIL
#define PR_FILTER_UTIL

#include <iostream>
#include <time.h>
#include <cmath>
#include <bits/stdc++.h>
#include "crypto_util.h"

using namespace CryptoPP;

#define ANOTHKEY "ANOTH"

struct cdc {
    std::vector<std::string> c;
    std::vector<std::string> dc;

    cdc() {}

    cdc(std::vector<std::string> c, std::vector<std::string> dc) : c(c), dc(dc) {}

    int size() { return c.size(); }
};

typedef struct cdc cdc;

typedef struct {
    std::map<std::string, cdc> EMMt;
    std::multiset<std::string> Xset;
} EMM;

typedef struct {
    std::string kx;
    std::vector<int> CK;
    std::vector<std::vector<int>> P2;
    std::vector<std::string> KeyPhi;
} key_re_d;

// get w1 and w2 from word
int find_w12_from_w(std::string word, std::string &w1, std::string &w2);

// arrange encryption: arrange pins in kep order, input is string
std::string Permutation(int n, std::vector<int> kep, std::string pin);

// rearrange encryption: rearrange pins back to the original column in kep order
std::string De_Permutation(int n, std::vector<int> kep, std::string dpin);

// arrange encryption: arrange pins in kep order
int Permutation2(int start, std::vector<int> kep, std::vector<std::string> pin, std::vector<std::string> &dpin);

// rearrange encryption: rearrange pins back to the original column in kep order, input is vector
int De_Permutation2(int start, std::vector<int> kep, std::vector<std::string> dpin, std::vector<std::string> &pin);

// adjust the sequence according to kpa and kpb
std::vector<int> Find_CK(int n, std::vector<int> kpa, std::vector<int> kpb);

// key extension: extends the key to n
int Permutationkey_Gen(std::string key, int n, std::vector<int> &ret);

int AONTH(int ctr, std::string m, std::string &mplus, std::string &dmplus);

int D_AONTH(int ctr, std::string mplus, std::string dmplus, std::string &m);

int Pr_Gen(std::vector<std::string> &key, std::vector<std::string> &w, int len, int doc, std::vector<int> &P1,
           std::vector<int> &P2, std::vector<int> &P3, std::string &keyphi);

int Pr_Enc(std::vector<std::string> &key, std::vector<std::string> &w, std::vector<std::string> &m, int len, std::map<std::string, int> &ZX,
           std::vector<std::string> &c, std::vector<std::string> &dc, std::map<std::string, int> &DX);

int Pr_Dec(std::vector<std::string> &key, std::vector<std::string> &w, std::vector<std::string> &c,
           std::vector<std::string> &dc, int len, std::map<std::string, int> &DX, std::vector<std::string> &m);

int Pr_ReGen(std::vector<std::string> &key, std::vector<std::string> &w, int len, std::vector<int> &RetCK3,
             std::vector<std::vector<int>> &RetP2, std::vector<std::string> &RetKeyPhi);

int Pr_ReEnc(std::vector<int> &CK3, std::vector<std::vector<int>> &P2, std::vector<std::string> &KeyPhi,
             std::vector<std::string> &c, std::vector<std::string> &dc, std::vector<std::string> &cplus,
             std::vector<std::string> &dcplus);

#endif // PR_FILTER_UTIL