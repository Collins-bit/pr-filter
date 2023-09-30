#ifndef CONJ_FILTER_H
#define CONJ_FILTER_H

#include <tk_filter/tk_filter_client.h>
#include <tk_filter/tk_filter_server.h>

using namespace std;

typedef struct {
    int lambda;
    map<string, vector<string>> MM;
} conj_filter_setup_param;

typedef struct {
    string kp;
    string kx;
    string kenc;
    string mskp;
    map<string, cdc> EMMp;
    multiset<string> X;
} conj_filter_setup_res;

int Conj_Filter_Setup(conj_filter_setup_param &param, conj_filter_setup_res &res);

typedef struct {
    string kp;
    string kx;
    string kenc;
    string mskp;
    vector<string> words;
} conj_filter_token_param;

typedef struct {
    string tokp;
    string k_l12_enc;
    vector<string> kx;
} conj_filter_token_res;

int Conj_Filter_Token(conj_filter_token_param &param, conj_filter_token_res &res);

typedef struct {
    string tokp;
    string k_l12_enc;
    vector<string> kx;
    map<string, cdc> EMMp;
    multiset<string> X;
} conj_filter_search_param;

int Conj_Filter_Search(conj_filter_search_param &param, vector<string> &ev);

typedef struct {
    string kenc;
    vector<string> ev;
} conj_filter_resolve_param;

int Conj_Filter_Resolve(conj_filter_resolve_param &param, vector<string> &ans);

#endif //CONJ_FILTER_H
