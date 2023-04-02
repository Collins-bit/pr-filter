#include <iostream>
#include <conj_filter/conj_filter.h>
#include <util/file_operate.h>
using namespace std;

int main() {
    map<string, vector<string>> MM = {
            {"name∩age",   {"id1", "id2", "id3"}},
            {"name∩sex", {"id1", "id2", "id3"}},
            {"age∩sex",  {"id1", "id2", "id3"}},

    };
    vector<string> words = {"name", "age", "sex"};
    // call Conj_Filter_Setup
    conj_filter_setup_param conj_filter_setup_param;
    conj_filter_setup_res conj_filter_setup_res;
    conj_filter_setup_param.MM = MM;
    conj_filter_setup_param.lambda = 32;

    if (Conj_Filter_Setup(conj_filter_setup_param, conj_filter_setup_res) != 0) {
        cout << "[setup] call Conj_Filter_Setup failed!" << endl;
        return -2;
    }
    cout << "Conj_Filter_Setup success!\n" << endl;

    // call Conj_Filter_Token
    conj_filter_token_param conj_filter_token_param;
    conj_filter_token_res conj_filter_token_res;
    conj_filter_token_param.kp = conj_filter_setup_res.kp;
    conj_filter_token_param.kx = conj_filter_setup_res.kx;
    conj_filter_token_param.kenc = conj_filter_setup_res.kenc;
    conj_filter_token_param.mskp = conj_filter_setup_res.mskp;
    conj_filter_token_param.words = words;
    if (Conj_Filter_Token(conj_filter_token_param, conj_filter_token_res) != 0) {
        cout << "[token] call Conj_Filter_Token failed!" << endl;
        return -2;
    }
    cout << "Conj_Filter_Token success!\n" << endl;

    // call Conj_Filter_Search
    conj_filter_search_param conj_filter_search_param;
    conj_filter_search_param.tokp = conj_filter_token_res.tokp;
    conj_filter_search_param.k_l12_enc = conj_filter_token_res.k_l12_enc;
    conj_filter_search_param.kx = conj_filter_token_res.kx;
    conj_filter_search_param.EMMp = conj_filter_setup_res.EMMp;
    conj_filter_search_param.X = conj_filter_setup_res.X;
    vector<string> conj_filter_ev;

    if (Conj_Filter_Search(conj_filter_search_param, conj_filter_ev) != 0) {
        cout << "call Conj_Filter_Search failed!" << endl;
        return -2;
    }
    cout << "Conj_Filter_Search success!\n" << endl;

    // call Conj_Filter_Resolve
    conj_filter_resolve_param conj_filter_resolve_param;
    conj_filter_resolve_param.kenc = conj_filter_setup_res.kenc;
    conj_filter_resolve_param.ev = conj_filter_ev;
    vector<string> conj_filter_ans;
    if (Conj_Filter_Resolve(conj_filter_resolve_param, conj_filter_ans) != 0) {
        cout << "call Conj_Filter_Resolve failed!" << endl;
        return -2;
    }
    StrVecRemoveTag(conj_filter_ans);
    cout << "Conj_Filter_Resolve result: ";
    for (const auto &ans: conj_filter_ans) {
        cout << ans << " ";
    }
    cout << endl;
    cout << "Conj_Filter_Resolve success!" << endl;
    return 0;
}