#ifndef PR_FILTER_CLIENT
#define PR_FILTER_CLIENT

#include "pr_filter_util.h"

using namespace CryptoPP;

typedef struct
{
    std::string kv;
    std::string kt;
    std::string kp;
    std::string kx;
    std::string kxor;
    std::string mskt;
} MK;

void sEMM_Setup(int lambda, std::map<std::string, cdc> MMp, std::string &mskt, std::map<std::string, cdc> &EMMp);

void sEMM_Token(std::string mskt, std::string w1, std::string w2, std::string &token);

typedef struct
{
    int lambda;
    int mu;
    std::map<std::string, std::vector<std::string>> MM;
} pr_filter_setup_param;

typedef struct
{
    MK mk;
    EMM emm;
    std::map<std::string, int> DX;
} pr_filter_setup_res;

int PR_Filter_Setup(pr_filter_setup_param param, pr_filter_setup_res &res);

typedef struct
{
    MK mk;
    std::vector<std::string> words;
    int len;
} pr_filter_token_param;

typedef struct
{
    std::string tokp;
    std::string k_w12_enc;
    std::vector<key_re_d> tokp_vec;
} pr_filter_token_res;

int PR_Filter_Token(pr_filter_token_param param, pr_filter_token_res &res);

typedef struct
{
    std::string w1;
    std::string wn;
    MK mk;
    std::vector<std::string> c;
    std::vector<std::string> dc;
    std::vector<bool> vaild;
    std::map<std::string, int> DX;
} pr_filter_resolve_param;

int PR_Filter_Resolve(pr_filter_resolve_param param, std::vector<std::string> &res);

#endif //PR_FILTER_CLIENT
