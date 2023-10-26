#include <iostream>
#include <conj_filter/conj_filter.h>
#include <util/file_operate.h>
#include <util/time_util.h>
#include <util/storage_overhead.h>
#include "test_utils.h"
using namespace std;

int main() {
    std::string fpath = "../testData/test_10_6.txt";
    int keylen = 32;
    // get mm from txt
    std::map<std::string, std::vector<std::string>> MM;
    int max_mm_len;
    Get_MM_from_fpath(fpath, MM, max_mm_len);
    Deal_mm_same_length(max_mm_len, MM);
    vector<string> words = get_words_from_mm(MM);

    // call Conj_Filter_Setup
    conj_filter_setup_param conj_setup_param;
    conj_filter_setup_res conj_setup_res;
    conj_setup_param.MM = MM;
    conj_setup_param.lambda = keylen;
    TimeUtil time_util1;
    time_util1.initTime();
    for (int i = 0; i < 1; i++) {
        if (Conj_Filter_Setup(conj_setup_param, conj_setup_res) != 0) {
            std::cout << "[setup] call Conj_Filter_Setup failed!" << std::endl;
            return -2;
        }
    }
    time_util1.endTime("conj setup process", 1);
    emmtChange2File(conj_setup_res.EMMp, "./emmt.txt");
    xsetChange2File(conj_setup_res.X, "./xset.txt");
    std::cout << "setup success!\n" << std::endl;

    // call Conj_Filter_Token
    conj_filter_token_param conj_token_param;
    conj_filter_token_res conj_token_res;
    conj_token_param.kp = conj_setup_res.kp;
    conj_token_param.kx = conj_setup_res.kx;
    conj_token_param.kenc = conj_setup_res.kenc;
    conj_token_param.mskp = conj_setup_res.mskp;
    conj_token_param.words = words;
    if (Conj_Filter_Token(conj_token_param, conj_token_res) != 0) {
        std::cout << "[token] call Conj_Filter_Token failed!" << std::endl;
        return -2;
    }
    conjTokenChange2File(conj_token_res, "./conj_token.txt");
    std::cout << "token success!\n" << std::endl;

    // call Conj_Filter_Search
    conj_filter_search_param conj_search_param;
    conj_search_param.tokp = conj_token_res.tokp;
    conj_search_param.k_l12_enc = conj_token_res.k_l12_enc;
    conj_search_param.kx = conj_token_res.kx;
    conj_search_param.EMMp = conj_setup_res.EMMp;
    conj_search_param.X = conj_setup_res.X;
    std::vector<std::string> conj_ev;
    TimeUtil time_util2;
    time_util2.initTime();
    for (int i = 0; i < 10; i++) {
        if (Conj_Filter_Search(conj_search_param, conj_ev) != 0) {
            std::cout << "call Conj_Filter_Search failed!" << std::endl;
            return -2;
        }
    }
    time_util2.endTime("search process", 10);
    std::cout << "search success!\n" << std::endl;

    // call Conj_Filter_Resolve
    conj_filter_resolve_param conj_resolve_param;
    conj_resolve_param.kenc = conj_setup_res.kenc;
    conj_resolve_param.ev = conj_ev;
    std::vector<std::string> conj_ans;
    if (Conj_Filter_Resolve(conj_resolve_param, conj_ans) != 0) {
        std::cout << "call Conj_Filter_Resolve failed!" << std::endl;
        return -2;
    }
    StrVecRemoveTag(conj_ans);
    std::cout << "resolve result: ";
    for (const auto &ans: conj_ans) {
        cout << ans << " ";
    }
    std::cout << std::endl;
    std::cout << "resolve success!" << std::endl;
    return 0;
}
