#ifndef PR_FILTER_CLIENT
#define PR_FILTER_CLIENT

#include "pr_filter_util.h"

using namespace CryptoPP;
using namespace std;

typedef struct {
    string kv;
    string kt;
    string kp;
    string kx;
    string kxor;
    string mskt;
} MK;

void sEMM_Setup(int lambda, map<string, cdc> &MMp, string &mskt, map<string, cdc> &EMMp);

void sEMM_Token(string mskt, string w1, string w2, string &token);

typedef struct {
    int lambda;
    int mu;
    map<string, vector<string>> MM;
} pr_filter_setup_param;

typedef struct {
    MK mk;
    EMM emm;
    map<string, int> DX;
} pr_filter_setup_res;

int PR_Filter_Setup(pr_filter_setup_param &param, pr_filter_setup_res &res);

typedef struct {
    MK mk;
    vector<string> words;
    int len;
} pr_filter_token_param;

typedef struct {
    string tokp;
    string k_w12_enc;
    vector<key_re_d> tokp_vec;
} pr_filter_token_res;

int PR_Filter_Token(pr_filter_token_param &param, pr_filter_token_res &res);

typedef struct {
    string w1;
    string wn;
    MK mk;
    vector<string> c;
    vector<string> dc;
    map<string, int> DX;
} pr_filter_resolve_param;

int PR_Filter_Resolve(pr_filter_resolve_param &param, vector<string> &res);

#endif //PR_FILTER_CLIENT
