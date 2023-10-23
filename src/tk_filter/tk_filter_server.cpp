#include <tk_filter/tk_filter_server.h>

void sEMM_Search(string tokp, map<string, vector<c_ectr>> &EMMp, vector<c_ectr> &tags) {
    tags = EMMp[tokp];
}

int TK_Filter_Search(tk_filter_search_param &param, tk_filter_search_res &res) {
    // tagl3 = sEMM.Search(tokp, EMMp)
    vector<c_ectr> tag_l3;
    sEMM_Search(param.tokp, param.emm.EMMt, tag_l3);
    // optimization
    // for l=1..L
    // for d=3..n   
    // c_ld = TK.ReEnc(ck_d, c_ld-1)
    vector<bool> flag(tag_l3.size(), true);
    for (int d = 0; d < param.ck.size(); d++) {
        for (int l = 0; l < tag_l3.size(); l++) {
            string c = tag_l3[l].c;
            string c_next;
            if (TK_ReEnc(param.ck[d], c, c_next) != 0) {
                cout << "[TK_Filter_Search] call TK_ReEnc failed" << endl;
                return -1;
            }
            if (param.emm.X.find(c_next) == param.emm.X.end()) {
                flag[l] = false;
                continue;
            }
            tag_l3[l].c = c_next;
        }
    }
    for (int l = 0; l < tag_l3.size(); l++) {
        if (flag[l]) {
            res.ct.push_back(tag_l3[l]);
        }
    }
    // done
    return 0;
}