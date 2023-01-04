#ifndef PRFILTER_CONJ_FILTER_H
#define PRFILTER_CONJ_FILTER_H

#include <algorithm/pr_filter_client.h>
#include <algorithm/pr_filter_server.h>

typedef struct {
    int lambda;
    std::map<std::string, std::vector<std::string>> MM;
} conj_filter_setup_param;

typedef struct {
    std::string kp;
    std::string kx;
    std::string kenc;
    std::string mskp;
    std::map<std::string, cdc> EMMp;
    std::multiset<std::string> X;
} conj_filter_setup_res;

int Conj_Filter_Setup(conj_filter_setup_param &param, conj_filter_setup_res &res);

typedef struct {
    std::string kp;
    std::string kx;
    std::string kenc;
    std::string mskp;
    std::vector<std::string> words;
} conj_filter_token_param;

typedef struct {
    std::string tokp;
    std::string k_l12_enc;
    std::vector<std::string> kx;
} conj_filter_token_res;

int Conj_Filter_Token(conj_filter_token_param &param, conj_filter_token_res &res);

typedef struct {
    std::string tokp;
    std::string k_l12_enc;
    std::vector<std::string> kx;
    std::map<std::string, cdc> EMMp;
    std::multiset<std::string> X;
} conj_filter_search_param;

int Conj_Filter_Search(conj_filter_search_param &param, std::vector<std::string> &ev);

typedef struct {
    std::string kenc;
    std::vector<std::string> ev;
} conj_filter_resolve_param;

int Conj_Filter_Resolve(conj_filter_resolve_param &param, std::vector<std::string> &ans);

#endif //PRFILTER_CONJ_FILTER_H
