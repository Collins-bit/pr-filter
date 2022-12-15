#ifndef PR_FILTER_SERVER
#define PR_FILTER_SERVER

#include "pr_filter_util.h"

using namespace CryptoPP;

void sEMM_Search(std::string tokp, std::map<std::string, cdc> EMMp, std::vector<std::string> &tags);

typedef struct
{
    std::string tokp;
    std::string k_w12_enc;
    std::vector<key_re_d> tokp_vec;
    EMM emm;
} pr_filter_search_param;

typedef struct
{
    std::vector<std::string> c;
    std::vector<std::string> dc;
    std::vector<bool> vaild;
} pr_filter_search_res;

int PR_Filter_Search(pr_filter_search_param param, pr_filter_search_res &res);

#endif //PR_FILTER_SERVER

