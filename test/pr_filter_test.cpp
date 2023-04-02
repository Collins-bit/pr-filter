#include <iostream>
#include <pr_filter/pr_filter_util.h>
#include <pr_filter/pr_filter_client.h>
#include <pr_filter/pr_filter_server.h>
#include <util/file_operate.h>
using namespace std;

int main() {
    map<string, vector<string>> MM = {
            {"name∩age",   {"id1", "id2", "id3"}},
            {"name∩sex", {"id1", "id2", "id3"}},
            {"age∩sex",  {"id1", "id2", "id3"}},

    };
    vector<string> words = {"name", "age", "sex"};

    // call PR_Filter_Setup
    pr_filter_setup_param pr_filter_setup_param;
    pr_filter_setup_res pr_filter_setup_res;
    pr_filter_setup_param.MM = MM;
    pr_filter_setup_param.lambda = 32;
    pr_filter_setup_param.mu = 32;
    if (PR_Filter_Setup(pr_filter_setup_param, pr_filter_setup_res) != 0) {
        cout << "[setup] call PR_Filter_Setup failed!" << endl;
        return -2;
    }
    cout << "PR_Filter_Setup success!\n" << endl;

    // call PR_Filter_Token
    pr_filter_token_param pr_filter_token_param;
    pr_filter_token_res pr_filter_token_res;
    pr_filter_token_param.mk = pr_filter_setup_res.mk;
    pr_filter_token_param.words = words;
    pr_filter_token_param.len = 3;
    if (PR_Filter_Token(pr_filter_token_param, pr_filter_token_res) != 0) {
        cout << "[token] call PR_Filter_Token failed!" << endl;
        return -2;
    }
    cout << "PR_Filter_Token success!\n" << endl;

    // call PR_Filter_Search
    pr_filter_search_param pr_filter_search_param;
    pr_filter_search_param.tokp = pr_filter_token_res.tokp;
    pr_filter_search_param.k_w12_enc = pr_filter_token_res.k_w12_enc;
    pr_filter_search_param.tokp_vec = pr_filter_token_res.tokp_vec;
    pr_filter_search_param.emm = pr_filter_setup_res.emm;
    pr_filter_search_res pr_filter_search_res;
    if (PR_Filter_Search(pr_filter_search_param, pr_filter_search_res) != 0) {
        cout << "call PR_Filter_Search failed!" << endl;
        return -2;
    }
    cout << "PR_Filter_Search success!\n" << endl;

    // call PR_Filter_Resolve
    pr_filter_resolve_param pr_filter_resolve_param;
    pr_filter_resolve_param.w1 = words[0];
    pr_filter_resolve_param.wn = words[words.size()-1];
    pr_filter_resolve_param.c = pr_filter_search_res.c;
    pr_filter_resolve_param.dc = pr_filter_search_res.dc;
    pr_filter_resolve_param.mk = pr_filter_setup_res.mk;
    pr_filter_resolve_param.DX = pr_filter_setup_res.DX;
    vector<string> pr_filter_ans;
    if (PR_Filter_Resolve(pr_filter_resolve_param, pr_filter_ans) != 0) {
        cout << "call PR_Filter_Resolve failed!" << endl;
        return -2;
    }
    StrVecRemoveTag(pr_filter_ans);
    cout << "PR_Filter_Resolve result: ";
    for (const auto &ans: pr_filter_ans) {
        cout << ans << " ";
    }
    cout << endl;
    cout << "PR_Filter_Resolve success!" << endl;
    return 0;
}