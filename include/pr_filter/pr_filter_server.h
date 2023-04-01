#ifndef PR_FILTER_SERVER
#define PR_FILTER_SERVER

#include "pr_filter_util.h"

using namespace CryptoPP;
using namespace std;

void sEMM_Search(string tokp, map<string, cdc> &EMMp, cdc &tags);

typedef struct {
    string tokp;
    string k_w12_enc;
    vector<key_re_d> tokp_vec;
    EMM emm;
} pr_filter_search_param;

typedef struct {
    vector<string> c;
    vector<string> dc;
} pr_filter_search_res;

int PR_Filter_Search(pr_filter_search_param &param, pr_filter_search_res &res);

#endif //PR_FILTER_SERVER

