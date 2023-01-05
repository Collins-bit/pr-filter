#include <algorithm/pr_filter_server.h>

void sEMM_Search(string tokp, map<string, cdc> &EMMp, cdc &tags) {
    tags = EMMp[tokp];
}

int PR_Filter_Search(pr_filter_search_param &param, pr_filter_search_res &res) {
    // tagl = sEMM.Search(tokp, EMMp)
    cdc tags_l;
    sEMM_Search(param.tokp, param.emm.EMMt, tags_l);
    // ci = Dec(k_w1_w2_enc, tagl)
    vector<string> c(tags_l.size());
    vector<string> dc(tags_l.size());
    for (int i = 0; i < tags_l.size(); i++) {
        decrypt(param.k_w12_enc, tags_l.c[i], c[i]);
        decrypt(param.k_w12_enc, tags_l.dc[i], dc[i]);
    }
    // optimization
    // for l=1..L
    // for d=3..nl
    // cl = PR.ReEnc(k_re_d, cl)
    // dtage(l,d) = F(kx, cl')
    int s = c.size();
    vector<string> cplus(s);
    vector<string> dcplus(s);
    string dtag1, dtag2;
    for (int i = 0; i < param.tokp_vec.size(); i++) {
        if (s == 0) {
            break;
        }
        if (Pr_ReEnc(param.tokp_vec[i].CK, param.tokp_vec[i].P2, param.tokp_vec[i].KeyPhi, c, dc, cplus, dcplus) != 0) {
            cout << "[PR_Filter_Search] call Pr_ReEnc failed" << endl;
            return -1;
        }
        for (int j = 0; j < s; j++) {
            dtag1 = H1(param.tokp_vec[i].kx, cplus[j]);
            dtag2 = H1(param.tokp_vec[i].kx, dcplus[j]);
            if (param.emm.Xset.find(dtag1 + dtag2) == param.emm.Xset.end()) {
                cplus.erase(cplus.begin() + j);
                dcplus.erase(dcplus.begin() + j);
                c.pop_back();
                dc.pop_back();
                --j;
                --s;
            }
        }
        for (int j = 0; j < s; j++) {
            c[j] = cplus[j];
            dc[j] = dcplus[j];
        }
    }
    res.c = cplus;
    res.dc = dcplus;
    // done
    return 0;
}