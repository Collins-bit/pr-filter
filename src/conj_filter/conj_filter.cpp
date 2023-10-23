#include <conj_filter/conj_filter.h>

void conj_sEMM_Setup(int lambda, map<string, cdc> &MMp, string &mskt, map<string, cdc> &EMMp) {
    mskt = Gen_RandKey(lambda);
    for (const auto& mmp: MMp) {
        string key_cipher;
        encrypt(mskt, mmp.first, key_cipher);
        EMMp[key_cipher] = mmp.second;
    }
}

void conj_sEMM_Search(string tokp, map<string, cdc> &EMMp, cdc &tags) {
    tags = EMMp[tokp];
}

int Conj_Filter_Setup(conj_filter_setup_param &param, conj_filter_setup_res &res) {
    // clear EMM and DX
    res.EMMp.clear();
    res.X.clear();
    // init rand
    srand(time(nullptr));
    // generate random key
    res.kp = Gen_RandKey(param.lambda);
    string kt = Gen_RandKey(param.lambda);
    res.kx = Gen_RandKey(param.lambda);
    res.kenc = Gen_RandKey(param.lambda);
    // init mmt and dx
    map<string, cdc> MMt;
    map<string, int> ZX;
    for (auto mm: param.MM) {
        // get a b
        string a, b;
        if (find_w12_from_w(mm.first, a, b) != 0) {
            cout << "[Conj_Filter_Setup] call find_w12_from_w err: " << mm.first << endl;
            return -1;
        }
        // keyt_a = F(Kt, a)
        string key_a_t = H1(kt, a);
        // keyab_enc=F(Kp, a||b)
        string key_ab_enc = H1(res.kp, a + b);
        // keyab_x=F(Kp, a||b)
        string key_ab_x = H1(res.kx, a + b);

        vector<string> etag(mm.second.size());
        vector<string> ev(mm.second.size());
        for (int i = 0; i < mm.second.size(); i++) {
            // tag = F(ka_t, v)
            string tag = H1(key_a_t, mm.second[i]);
            // etag = Enc(keyab_enc, tag)
            encrypt(key_ab_enc, tag, etag[i]);
            // ev = Enc(kenc, v)
            encrypt(res.kenc, mm.second[i], ev[i]);
            // K <- F(keyab_x, tag)
            res.X.insert(H1(key_ab_x, tag));
        }
        MMt[mm.first] = cdc(etag, ev);
    }
    // mskt, EMMp = sEMM_Setup(1_lamda, MMp)
    conj_sEMM_Setup(param.lambda, MMt, res.mskp, res.EMMp);
    return 0;
}

int Conj_Filter_Token(conj_filter_token_param &param, conj_filter_token_res &res) {
    //clear kx
    res.kx.clear();
    if (param.words.size() < 2) {
        cout << "[Conj_Filter_Token] words size too little err: " << param.words.size() << endl;
        return -1;
    }
    // tokp1=sEMM.Token(mskp, (w1, w2))
    sEMM_Token(param.mskp, param.words[0], param.words[1], res.tokp);
    // k_w12_enc = hash(kp, w1+w2)
    res.k_l12_enc = H1(param.kp, param.words[0] + param.words[1]);
    for (int i = 2; i < param.words.size(); i++) {
        // kx = F(kx, w1+wd)
        string kx = H1(param.kx, param.words[0] + param.words[i]);
        res.kx.push_back(kx);
    }
    // done
    return 0;
}

int Conj_Filter_Search(conj_filter_search_param &param, vector<string> &ev) {
    // clear ans
    ev.clear();
    // tagl = sEMM.Search(tokp, EMMp)
    cdc tags_l;
    conj_sEMM_Search(param.tokp, param.EMMp, tags_l);
    // tagi = Dec(k_l12_enc, etagl)
    vector<string> tag(tags_l.c.size());
    for (int i = 0; i < tags_l.c.size(); i++) {
        decrypt(param.k_l12_enc, tags_l.c[i], tag[i]);
    }
    // dtag(l,d) = F(K_x_l, tagd)
    for (int l = 0; l < tag.size(); l++) {
        bool flag = true;
        for (int d = 0; d < param.kx.size(); d++) {
            string dtag = H1(param.kx[d], tag[l]);
            if (param.X.find(dtag) == param.X.end()) {
                flag = false;
                break;
            }
        }
        if (flag) ev.push_back(tags_l.dc[l]);
    }
    // done
    return 0;
}

int Conj_Filter_Resolve(conj_filter_resolve_param &param, vector<string> &ans) {
    for (const auto &ev: param.ev) {
        string a;
        decrypt(param.kenc, ev, a);
        ans.push_back(a);
    }
    // done
    return 0;
}