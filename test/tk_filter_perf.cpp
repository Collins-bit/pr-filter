#include <iostream>
#include <conj_filter/conj_filter.h>
#include <util/file_operate.h>
#include <util/time_util.h>
#include <util/storage_overhead.h>
#include "test_utils.h"
using namespace std;

int main() {
    std::string fpath = "../testData/test_MM.txt";
    int keylen = 32;
    // get mm from txt
    std::map<std::string, std::vector<std::string>> MM;
    int max_mm_len;
    Get_MM_from_fpath(fpath, MM, max_mm_len);
    Deal_mm_same_length(max_mm_len, MM);
    vector<string> words = get_words_from_mm(MM);

    // call Conj_Filter_Setup
    tk_filter_setup_param tk_filter_setup_param;
    tk_filter_setup_res tk_filter_setup_res;
    tk_filter_setup_param.MM = MM;
    tk_filter_setup_param.lambda = keylen;
    TimeUtil time_util1;
    time_util1.initTime();
    for (int i = 0; i < 1; i++) {
        if (TK_Filter_Setup(tk_filter_setup_param, tk_filter_setup_res) != 0) {
            std::cout << "[setup] call TK_Filter_Setup failed!" << std::endl;
            return -2;
        }
    }
    time_util1.endTime("conj setup process", 100);
    emmtChange2File(tk_filter_setup_res.emm.EMMt, "./emmt.txt");
    xsetChange2File(tk_filter_setup_res.emm.X, "./xset.txt");
    std::cout << "setup success!\n" << std::endl;

    // call Conj_Filter_Token
    tk_filter_token_param tk_filter_token_param;
    tk_filter_token_res tk_filter_token_res;
    tk_filter_token_param.mk = tk_filter_setup_res.mk;
    tk_filter_token_param.words = words;
    tk_filter_token_param.len = max_mm_len;
    if (TK_Filter_Token(tk_filter_token_param, tk_filter_token_res) != 0) {
        std::cout << "[token] call TK_Filter_Token failed!" << std::endl;
        return -2;
    }
    tokenChange2File(tk_filter_token_res, "./conj_token.txt");
    std::cout << "token success!\n" << std::endl;

    // call Conj_Filter_Search
    tk_filter_search_param tk_filter_search_param;
    tk_filter_search_param.tokp = tk_filter_token_res.tokp;
    tk_filter_search_param.ck = tk_filter_token_res.ck;
    tk_filter_search_param.emm = tk_filter_setup_res.emm;
    tk_filter_search_res tk_filter_search_res;
    TimeUtil time_util2;
    time_util2.initTime();
    for (int i = 0; i < 1; i++) {
        if (TK_Filter_Search(tk_filter_search_param, tk_filter_search_res)!= 0) {
            std::cout << "call TK_Filter_Search failed!" << std::endl;
            return -2;
        }
    }
    time_util2.endTime("search process", 10000);
    std::cout << "search success!\n" << std::endl;

    // call Conj_Filter_Resolve
    tk_filter_resolve_param tk_filter_resolve_param;
    tk_filter_resolve_param.w1 = words[0];
    tk_filter_resolve_param.wn = words[words.size()-1];
    tk_filter_resolve_param.ct = tk_filter_search_res.ct;
    tk_filter_resolve_param.mk = tk_filter_setup_res.mk;
    vector<string> tk_filter_ans;
    if (TK_Filter_Resolve(tk_filter_resolve_param, tk_filter_ans) != 0) {
        std::cout << "call Conj_Filter_Resolve failed!" << std::endl;
        return -2;
    }
    StrVecRemoveTag(tk_filter_ans);
    std::cout << "resolve result: ";
    for (const auto &ans: tk_filter_ans) {
        cout << ans << " ";
    }
    std::cout << std::endl;
    std::cout << "resolve success!" << std::endl;
    return 0;
}
