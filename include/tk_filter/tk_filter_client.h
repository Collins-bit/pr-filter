#ifndef TK_FILTER_CLIENT
#define TK_FILTER_CLIENT

#include "tk_filter_util.h"

using namespace CryptoPP;
using namespace std;

typedef struct {
    string kt;
    string kp;
    string mskt;
} MK;

void sEMM_Setup(int lambda, map<string, c_ectr> &MMp, string &mskt, map<string, c_ectr> &EMMp);

void sEMM_Token(string mskt, string w1, string w2, string &token);

typedef struct {
    int lambda;
    map<string, vector<string>> MM;
} tk_filter_setup_param;

typedef struct {
    MK mk;
    EMM emm;
} tk_filter_setup_res;

int TK_Filter_Setup(tk_filter_setup_param &param, tk_filter_setup_res &res);

typedef struct {
    MK mk;
    vector<string> words;
    int len;
} tk_filter_token_param;

typedef struct {
    string tokp;
    vector<vector<int>> ck;
} tk_filter_token_res;

int TK_Filter_Token(tk_filter_token_param &param, tk_filter_token_res &res);

typedef struct {
    string w1;
    string wn;
    vector<c_ectr> ct;
    MK mk;
} tk_filter_resolve_param;

int TK_Filter_Resolve(tk_filter_resolve_param &param, vector<string> &res);

#endif //TK_FILTER_CLIENT
