#include <pr_filter/pr_filter_client.h>

void sEMM_Setup(int lambda, map<string, cdc> &MMp, string &mskt, map<string, cdc> &EMMp) {
    mskt = Gen_RandKey(lambda);
    for (const auto& mmp: MMp) {
        string key_cipher;
        encrypt(mskt, mmp.first, key_cipher);
        EMMp[key_cipher] = mmp.second;
    }
}

void sEMM_Token(string mskt, string w1, string w2, string &token) {
    string word = w1 + "∩" + w2;
    encrypt(mskt, word, token);
}

int PR_Filter_Setup(pr_filter_setup_param &param, pr_filter_setup_res &res) {
    // clear EMM and DX
    res.emm.EMMt.clear();
    res.emm.Xset.clear();
    res.DX.clear();
    // init rand
    srand(time(nullptr));
    // generate random key
    res.mk.kv = Gen_RandKey(param.lambda);
    res.mk.kt = Gen_RandKey(param.lambda);
    res.mk.kp = Gen_RandKey(param.lambda);
    res.mk.kx = Gen_RandKey(param.lambda);
    res.mk.kxor = Gen_RandKey(param.mu);
    // init mmt and dx
    map<string, cdc> MMt;
    map<string, int> ZX;
    for (auto mm: param.MM) {
        int s = mm.second.size();
        // get a b
        string a, b;
        if (find_w12_from_w(mm.first, a, b) != 0) {
            cout << "[PR_Filter_Setup] call find_w12_from_w err: " << mm.first << endl;
            return -1;
        }
        // keyab_enc=F(Kp, a||b)
        string key_ab_enc = H1(res.mk.kp, a + b);
        // (c1,dc1)..(cs,dcs) = Pr.Enc(Kv, kt, kxor, a, b, v1...vs, len, s)
        vector<string> c(s);
        vector<string> dc(s);
        vector<string> key{res.mk.kv, res.mk.kt, res.mk.kxor};
        vector<string> w{a, b};
        int len = 0;
        if (s > 0) {
            len = mm.second[0].size();
        }
        for (int i = 0; i < mm.second.size(); i++) {
            if (ZX[mm.second[i]] == 0) {
                unsigned char ctr = rand() % 1000000;
                ZX[mm.second[i]] = ctr;
            }
        }
        if (Pr_Enc(key, w, mm.second, len, ZX, c, dc, res.DX) != 0) {
            cout << "[PR_Filter_Setup] call Pr_Enc failed" << endl;
            return -1;
        }
        // encrypt tag(a,b,v)=Aes_Enc(keyab_enc, (ci, dci))
        vector<string> ec(s);
        vector<string> edc(s);
        for (int i = 0; i < s; i++) {
            encrypt(key_ab_enc, c[i], ec[i]);
            encrypt(key_ab_enc, dc[i], edc[i]);
        }
        MMt[mm.first] = cdc(ec, edc);

        // keyab_x=F(Kp, a||b)
        string key_ab_x = H1(res.mk.kx, a + b);
        // tag <- xset
        for (int i = 0; i < s; i++) {
            res.emm.Xset.insert(H1(key_ab_x, c[i]) + H1(key_ab_x, dc[i]));
        }
    }
    // mskt, EMMp = sEMM_Setup(1_lamda, MMp)
    sEMM_Setup(param.lambda, MMt, res.mk.mskt, res.emm.EMMt);
    return 0;
}

int PR_Filter_Token(pr_filter_token_param &param, pr_filter_token_res &res) {
    if (param.words.size() < 2) {
        cout << "[PR_Filter_Token] words size too little err: " << param.words.size() << endl;
        return -1;
    }
    // tokp1=sEMM.Token(mskp, (w1, w2))
    sEMM_Token(param.mk.mskt, param.words[0], param.words[1], res.tokp);
    // k_w12_enc = hash(kp, w1+w2)
    res.k_w12_enc = H1(param.mk.kp, param.words[0] + param.words[1]);
    // n = word.size()
    int n = param.words.size();
    for (int i = 2; i < n; i++) {
        // kred = PR.ReGen(kv, kt, kxor, w1, wd-1, wd, len, s)
        vector<string> input_key{param.mk.kv, param.mk.kt, param.mk.kxor};
        vector<string> input_w{param.words[0], param.words[i - 1], param.words[i]};
        vector<int> CK;
        vector<vector<int>> P2(2);
        vector<string> KeyPhi(2);
        if (Pr_ReGen(input_key, input_w, param.len, CK, P2, KeyPhi) != 0) {
            cout << "[PR_Filter_Token] call Pr_ReGen failed" << endl;
            return -1;
        }
        // split
        key_re_d k_re_d;
        k_re_d.CK = CK;
        k_re_d.P2 = P2;
        k_re_d.KeyPhi = KeyPhi;
        // kxi = hash(kx, w1+wd)
        k_re_d.kx = H1(param.mk.kx, param.words[0] + param.words[i]);
        res.tokp_vec.push_back(k_re_d);
    }
    // done
    return 0;
}

int PR_Filter_Resolve(pr_filter_resolve_param &param, vector<string> &res) {
    if (param.c.size() != param.dc.size()) {
        cout << "[PR_Filter_Resolve] c and dc size wrong: " << param.c.size() << endl;
        return -1;
    }
    // v1..vs=PR.Dec(kv, kt, kxor, w1, wn, c0..cs)
    int s = param.c.size(), len = 0;
    if (s > 0) {
        len = param.c[0].size();
    }
    vector<string> v(s);
    vector<string> key{param.mk.kv, param.mk.kt, param.mk.kxor};
    vector<string> w{param.w1, param.wn};
    if (Pr_Dec(key, w, param.c, param.dc, len, param.DX, v) != 0) {
        cout << "[PR_Filter_Resolve] call Pr_Dec failed" << endl;
        return -1;
    }
    // get result
    for (int i = 0; i < s; i++) {
        res.push_back(v[i]);
    }
    // done
    return 0;
}