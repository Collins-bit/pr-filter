#ifndef PR_FILTER_UTIL
#define PR_FILTER_UTIL

#include <iostream>
#include <time.h>
#include <cmath>
#include <bits/stdc++.h>
#include "crypto_util.h"

using namespace CryptoPP;

#define ANOTHKEY "ANOTH"

struct cdc
{
    std::vector<std::string> c;
    std::vector<std::string> dc;
    cdc(){}
    cdc(std::vector<std::string> c, std::vector<std::string> dc): c(c), dc(dc) {}
    int size(){return c.size();}
};
typedef struct cdc cdc;

typedef struct
{
    std::map<std::string, cdc> EMMt;
    std::multiset<std::string> Xset;
} EMM;

typedef struct
{
    std::string kx;
    std::vector<int> CK;
    std::vector<std::vector<int>> P2;
    std::vector<std::string> KeyPhi;
} key_re_d;

//从word中获取w1和w2
int find_w12_from_w(std::string word, std::string &w1, std::string &w2);

// 排列加密算法: 将pin按kep顺序排列，pin为string
std::string Permutation(int n, std::vector<int> kep, std::string pin);

// 排列加密算法: 将pin按kep顺序重排回原列
std::string De_Permutation(int n, std::vector<int> kep, std::string dpin);

// 排列加密算法：将pin按kep顺序排列，pin为string数组
int Permutation2(int start, std::vector<int> kep, std::vector<std::string> pin, std::vector<std::string> &dpin);

// 排列加密算法: 将pin按kep顺序重排回原列
int De_Permutation2(int start, std::vector<int> kep, std::vector<std::string> dpin, std::vector<std::string> &pin);

// 根据kpa和kpb调整顺序
std::vector<int> Find_CK(int n, std::vector<int> kpa, std::vector<int> kpb);

// 密钥扩展函数，将密钥从key长度扩展到n长度
int Permutationkey_Gen(std::string key, int n, std::vector<int> &ret);

int AONTH(int ctr, std::string m, std::string &mplus, std::string &dmplus);

int D_AONTH(int ctr, std::string mplus, std::string dmplus, std::string &m);

int Pr_Gen(std::vector<std::string> key, std::vector<std::string> w, int len, int doc, std::vector<int> &P1, std::vector<int> &P2, std::vector<int> &P3, std::string &keyphi);

int Pr_Enc(std::vector<std::string> key, std::vector<std::string> w, std::vector<std::string> m, int len, std::map<std::string, int> ZX, std::vector<std::string> &c, std::vector<std::string> &dc, std::map<std::string, int> &DX);

int Pr_Dec(std::vector<std::string> key, std::vector<std::string> w, std::vector<std::string> c, std::vector<std::string> dc, int len, std::map<std::string, int> DX, std::vector<std::string> &m);

int Pr_ReGen(std::vector<std::string> key, std::vector<std::string> w, int len, std::vector<int> &RetCK3, std::vector<std::vector<int>> &RetP2, std::vector<std::string> &RetKeyPhi);

int Pr_ReEnc(std::vector<int> CK3, std::vector<std::vector<int>> P2, std::vector<std::string> KeyPhi, std::vector<std::string> c, std::vector<std::string> dc, std::vector<std::string> &cplus, std::vector<std::string> &dcplus);

#endif // PR_FILTER_UTIL