#ifndef TK_FILTER_SERVER
#define TK_FILTER_SERVER

#include "tk_filter_util.h"

using namespace CryptoPP;
using namespace std;

void sEMM_Search(string tokp, map<string, c_ectr> &EMMp, c_ectr &tags);

typedef struct {
    string tokp;
    vector<vector<int>> ck;
    EMM emm;

} tk_filter_search_param;

typedef struct {
    vector<c_ectr> ct;
} tk_filter_search_res;

int TK_Filter_Search(tk_filter_search_param &param, tk_filter_search_res &res);

#endif //TK_FILTER_SERVER

