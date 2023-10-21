#ifndef TK_FILTER_SERVER
#define TK_FILTER_SERVER

#include "tk_filter_util.h"

using namespace CryptoPP;
using namespace std;

void sEMM_Search(string tokp, map<string, cdc> &EMMp, cdc &tags);

typedef struct {
    string tokp;
    string k_w12_enc;
    vector<key_re_d> tokp_vec;
    EMM emm;
} tk_filter_search_param;

typedef struct {
    vector<string> c;
    vector<string> dc;
} tk_filter_search_res;

int TK_Filter_Search(tk_filter_search_param &param, tk_filter_search_res &res);

#endif //TK_FILTER_SERVER

