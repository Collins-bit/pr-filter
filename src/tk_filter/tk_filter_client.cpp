#include <tk_filter/tk_filter_client.h>

void sEMM_Setup(int lambda, map<string, vector<c_ectr>> &MMp, string &mskt, map<string, vector<c_ectr>> &EMMp) {
    mskt = Gen_RandKey(lambda);
    for (const auto &mmp: MMp) {
        string key_cipher;
        encrypt(mskt, mmp.first, key_cipher);
        EMMp[key_cipher] = mmp.second;
    }
}

void sEMM_Token(string mskt, string w1, string w2, string &token) {
    string word = w1 + "∩" + w2;
    encrypt(mskt, word, token);
}

int TK_Filter_Setup(tk_filter_setup_param &param, tk_filter_setup_res &res) {
    // clear EMM and X
    res.emm.EMMt.clear();
    res.emm.X.clear();
    // init rand
    srand(time(nullptr));
    // generate random key
    res.mk.kt = Gen_RandKey(param.lambda);
    res.mk.kp = Gen_RandKey(param.lambda);
    // init mmt
    map<string, vector<c_ectr>> MMt;
    for (auto mm: param.MM) {
        // get a b
        string a, b;
        if (find_w12_from_w(mm.first, a, b) != 0) {
            cout << "[TK_Filter_Setup] call find_w12_from_w err: " << mm.first << endl;
            return -1;
        }
        vector<c_ectr> first_c_ectr(mm.second.size());
        for (int i = 0; i < mm.second.size(); i++) {
            string v = mm.second[i];
            // ctr = Hash(a, v)
            string ctr = H1(a, v);
            string c;
            // c = tk_enc(kt, a, b, v)
            if (TK_Enc(res.mk.kt, a, b, v, c) != 0) {
                cout << "[TK_Filter_Setup] call TK_Enc failed" << endl;
                return -1;
            }
            string ectr;
            // ectr = enc(kp, ctr)
            encrypt(res.mk.kp, ctr, ectr);
            // add c_ectr
            first_c_ectr[i] = c_ectr(c, ectr);
            string hash_c = H(c);
            // add c into X
            res.emm.X.insert(hash_c);
        }
        MMt[mm.first] = first_c_ectr;
    }

    // mskt, EMMp = sEMM_Setup(1_lamda, MMp)
    sEMM_Setup(param.lambda, MMt, res.mk.mskt, res.emm.EMMt);
    return 0;
}

int TK_Filter_Token(tk_filter_token_param &param, tk_filter_token_res &res) {
    if (param.words.size() < 2) {
        cout << "[TK_Filter_Token] words size too little err: " << param.words.size() << endl;
        return -1;
    }
    // tokp = sEMM.Token(mskt, (w1, w2))
    sEMM_Token(param.mk.mskt, param.words[0], param.words[1], res.tokp);
    // n = word.size()
    int n = param.words.size();
    for (int i = 2; i < n; i++) {
        // ck_d = tk.re_gen(kt, l1, l2, ld, len)
        vector<int> ck(param.len);
        TK_ReGen(param.mk.kt, param.words[0], param.words[i-1], param.words[i], param.len, ck);
        res.ck.push_back(ck);
    }
    // done
    return 0;
}

int TK_Filter_Resolve(tk_filter_resolve_param &param, vector<string> &res) {
    res.clear();
    // v1..vs=TK.Dec(kv, kt, kxor, w1, wn, c0..cs)
    for (int i = 0; i < param.ct.size(); i++) {
        string v;
        TK_Dec(param.mk.kt, param.mk.kp, param.w1, param.wn, param.ct[i].c, param.ct[i].ectr, v);
        res.push_back(v);
    }
    // done
    return 0;
}