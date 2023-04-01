#ifndef PR_FILTER_UTIL
#define PR_FILTER_UTIL

#include <iostream>
#include <time.h>
#include <cmath>
#include <bits/stdc++.h>
#include "crypto_util.h"

using namespace CryptoPP;
using namespace std;

#define ANOTHKEY "ANOTH"

struct cdc {
    vector<string> c;
    vector<string> dc;

    cdc() {}

    cdc(vector<string> c, vector<string> dc) : c(c), dc(dc) {}

    int size() { return c.size(); }
};

typedef struct cdc cdc;

typedef struct {
    map<string, cdc> EMMt;
    multiset<string> Xset;
} EMM;

typedef struct {
    string kx;
    vector<int> CK;
    vector<vector<int>> P2;
    vector<string> KeyPhi;
} key_re_d;

// get w1 and w2 from word
int find_w12_from_w(string word, string &w1, string &w2);

// arrange encryption: arrange pins in kep order, input is string
string Permutation(int n, vector<int> kep, string pin);

// rearrange encryption: rearrange pins back to the original column in kep order
string De_Permutation(int n, vector<int> kep, string dpin);

// arrange encryption: arrange pins in kep order
int Permutation2(int start, vector<int> kep, vector<string> pin, vector<string> &dpin);

// rearrange encryption: rearrange pins back to the original column in kep order, input is vector
int De_Permutation2(int start, vector<int> kep, vector<string> dpin, vector<string> &pin);

// adjust the sequence according to kpa and kpb
vector<int> Find_CK(int n, vector<int> kpa, vector<int> kpb);

// key extension: extends the key to n
int Permutationkey_Gen(string key, int n, vector<int> &ret);

int AONTH(int ctr, string m, string &mplus, string &dmplus);

int D_AONTH(int ctr, string mplus, string dmplus, string &m);

int Pr_Gen(vector<string> &key, vector<string> &w, int len, int doc, vector<int> &P1,
           vector<int> &P2, vector<int> &P3, string &keyphi);

int Pr_Enc(vector<string> &key, vector<string> &w, vector<string> &m, int len, map<string, int> &ZX,
           vector<string> &c, vector<string> &dc, map<string, int> &DX);

int Pr_Dec(vector<string> &key, vector<string> &w, vector<string> &c,
           vector<string> &dc, int len, map<string, int> &DX, vector<string> &m);

int Pr_ReGen(vector<string> &key, vector<string> &w, int len, vector<int> &RetCK3,
             vector<vector<int>> &RetP2, vector<string> &RetKeyPhi);

int Pr_ReEnc(vector<int> &CK3, vector<vector<int>> &P2, vector<string> &KeyPhi,
             vector<string> &c, vector<string> &dc, vector<string> &cplus,
             vector<string> &dcplus);

#endif // PR_FILTER_UTIL