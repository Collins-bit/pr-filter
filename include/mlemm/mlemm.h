#ifndef MLEMM_H
#define MLEMM_H

#include <pr_filter/pr_filter_client.h>
#include <pr_filter/pr_filter_server.h>

using namespace std;

typedef struct {
    int lambda;
    map<string, vector<string>> MM;
} mlemm_setup_param;

typedef struct {
    string kp;
    string kx;
    string kenc;
    string mskp;
    map<string, cdc> EMMp;
    multiset<string> X;
} mlemm_setup_res;

int MLEMM_Setup(mlemm_setup_param &param, mlemm_setup_res &res);

typedef struct {
    string kp;
    string kx;
    string kenc;
    string mskp;
    vector<string> words;
} mlemm_token_param;

typedef struct {
    string tokp;
    string k_l123_enc;
    vector<string> kx;
} mlemm_token_res;

int MLEMM_Token(mlemm_token_param &param, mlemm_token_res &res);

typedef struct {
    string tokp;
    string k_l123_enc;
    vector<string> kx;
    map<string, cdc> EMMp;
    multiset<string> X;
} mlemm_search_param;

int MLEMM_Search(mlemm_search_param &param, vector<string> &ev);

typedef struct {
    string kenc;
    vector<string> ev;
} mlemm_resolve_param;

int MLEMM_Resolve(mlemm_resolve_param &param, vector<string> &ans);

#endif //MLEMM_H
