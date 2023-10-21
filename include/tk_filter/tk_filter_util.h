#ifndef TK_FILTER_UTIL
#define TK_FILTER_UTIL

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

int TK_Gen(vector<string> &key, vector<string> &w, int len, int doc, vector<int> &P1,
           vector<int> &P2, vector<int> &P3, string &keyphi);

int TK_Enc(string kt, string a, string b, string v, string &c);

int TK_Dec(string kt, string kp, string a, string b, string c, string ectr, string &v);

int TK_ReGen(string kt, string a, string b, string w, int l, vector<int> &ck_w);

int TK_ReEnc(vector<int> &ck_w, string c, string &c_plus);

#endif // TK_FILTER_UTIL