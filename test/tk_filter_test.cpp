#include <iostream>
#include <tk_filter/tk_filter_util.h>
#include <tk_filter/tk_filter_client.h>
#include <tk_filter/tk_filter_server.h>
#include <util/file_operate.h>
using namespace std;

int main() {
    map<string, vector<string>> MM = {
            {"name∩age",   {"id1", "id2", "id3"}},
            {"name∩sex", {"id1", "id2", "id3"}},
            {"age∩sex",  {"id1", "id2", "id3"}},

    };
    vector<string> words = {"name", "age", "sex"};

    // call TK_Filter_Setup
    tk_filter_setup_param tk_filter_setup_param;
    tk_filter_setup_res tk_filter_setup_res;
    tk_filter_setup_param.MM = MM;
    tk_filter_setup_param.lambda = 32;
    tk_filter_setup_param.mu = 32;
    if (TK_Filter_Setup(tk_filter_setup_param, tk_filter_setup_res) != 0) {
        cout << "[setup] call TK_Filter_Setup failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Setup success!\n" << endl;

    // call TK_Filter_Token
    tk_filter_token_param tk_filter_token_param;
    tk_filter_token_res tk_filter_token_res;
    tk_filter_token_param.mk = tk_filter_setup_res.mk;
    tk_filter_token_param.words = words;
    tk_filter_token_param.len = 3;
    if (TK_Filter_Token(tk_filter_token_param, tk_filter_token_res) != 0) {
        cout << "[token] call TK_Filter_Token failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Token success!\n" << endl;

    // call TK_Filter_Search
    tk_filter_search_param tk_filter_search_param;
    tk_filter_search_param.tokp = tk_filter_token_res.tokp;
    tk_filter_search_param.k_w12_enc = tk_filter_token_res.k_w12_enc;
    tk_filter_search_param.tokp_vec = tk_filter_token_res.tokp_vec;
    tk_filter_search_param.emm = tk_filter_setup_res.emm;
    tk_filter_search_res tk_filter_search_res;
    if (TK_Filter_Search(tk_filter_search_param, tk_filter_search_res) != 0) {
        cout << "call TK_Filter_Search failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Search success!\n" << endl;

    // call TK_Filter_Resolve
    tk_filter_resolve_param tk_filter_resolve_param;
    tk_filter_resolve_param.w1 = words[0];
    tk_filter_resolve_param.wn = words[words.size()-1];
    tk_filter_resolve_param.c = tk_filter_search_res.c;
    tk_filter_resolve_param.dc = tk_filter_search_res.dc;
    tk_filter_resolve_param.mk = tk_filter_setup_res.mk;
    tk_filter_resolve_param.DX = tk_filter_setup_res.DX;
    vector<string> tk_filter_ans;
    if (TK_Filter_Resolve(tk_filter_resolve_param, tk_filter_ans) != 0) {
        cout << "call TK_Filter_Resolve failed!" << endl;
        return -2;
    }
    StrVecRemoveTag(tk_filter_ans);
    cout << "TK_Filter_Resolve result: ";
    for (const auto &ans: tk_filter_ans) {
        cout << ans << " ";
    }
    cout << endl;
    cout << "TK_Filter_Resolve success!" << endl;
    return 0;
}