#include <iostream>
#include <mlemm/mlemm.h>
#include <util/file_operate.h>
using namespace std;

int main() {
    map<string, vector<string>> MM = {
            {"name∩age∩sex",   {"id1", "id2", "id3"}},
            {"name∩age∩grade", {"id1", "id2", "id3"}},
            {"age∩sex∩grade",  {"id1", "id2", "id3"}},

    };
    vector<string> words = {"name", "age", "sex", "grade"};
    // call MLEMM_Setup
    mlemm_setup_param mlemm_setup_param;
    mlemm_setup_res mlemm_setup_res;
    mlemm_setup_param.MM = MM;
    mlemm_setup_param.lambda = 32;

    if (MLEMM_Setup(mlemm_setup_param, mlemm_setup_res) != 0) {
        cout << "[setup] call MLEMM_Setup failed!" << endl;
        return -2;
    }
    cout << "MLEMM_Setup success!\n" << endl;

    // call MLEMM_Token
    mlemm_token_param mlemm_token_param;
    mlemm_token_res mlemm_token_res;
    mlemm_token_param.kp = mlemm_setup_res.kp;
    mlemm_token_param.kx = mlemm_setup_res.kx;
    mlemm_token_param.kenc = mlemm_setup_res.kenc;
    mlemm_token_param.mskp = mlemm_setup_res.mskp;
    mlemm_token_param.words = words;
    if (MLEMM_Token(mlemm_token_param, mlemm_token_res) != 0) {
        cout << "[token] call MLEMM_Token failed!" << endl;
        return -2;
    }
    cout << "MLEMM_Token success!\n" << endl;

    // call MLEMM_Search
    mlemm_search_param mlemm_search_param;
    mlemm_search_param.tokp = mlemm_token_res.tokp;
    mlemm_search_param.k_l123_enc = mlemm_token_res.k_l123_enc;
    mlemm_search_param.kx = mlemm_token_res.kx;
    mlemm_search_param.EMMp = mlemm_setup_res.EMMp;
    mlemm_search_param.X = mlemm_setup_res.X;
    vector<string> mlemm_ev;

    if (MLEMM_Search(mlemm_search_param, mlemm_ev) != 0) {
        cout << "call MLEMM_Search failed!" << endl;
        return -2;
    }
    cout << "MLEMM_Search success!\n" << endl;

    // call MLEMM_Resolve
    mlemm_resolve_param mlemm_resolve_param;
    mlemm_resolve_param.kenc = mlemm_setup_res.kenc;
    mlemm_resolve_param.ev = mlemm_ev;
    vector<string> mlemm_ans;
    if (MLEMM_Resolve(mlemm_resolve_param, mlemm_ans) != 0) {
        cout << "call MLEMM_Resolve failed!" << endl;
        return -2;
    }
    StrVecRemoveTag(mlemm_ans);
    cout << "MLEMM_Resolve result: ";
    for (const auto &ans: mlemm_ans) {
        cout << ans << " ";
    }
    cout << endl;
    cout << "MLEMM_Resolve success!" << endl;
    return 0;
}