#include <mlemm/mlemm.h>

int find_w123_from_w(const string &word, string &w1, string &w2, string &w3) {
    size_t pos1 = word.find("∩");
    if (pos1 == string::npos) {
        // 没有找到第一个分割符
        return -1;
    }
    w1 = word.substr(0, pos1);
    size_t pos2 = word.find("∩", pos1 + 3);
    if (pos2 == string::npos) {
        // 没有找到第二个分割符
        return -1;
    }
    w2 = word.substr(pos1 + 3, pos2 - pos1 - 3);
    w3 = word.substr(pos2 + 3);
    return 0;
}

void sEMM_Token_w3(string mskt, string w1, string w2, string w3, string &token) {
    string word = w1 + "∩" + w2 + "∩" +w3;
    encrypt(mskt, word, token);
}

int MLEMM_Setup(mlemm_setup_param &param, mlemm_setup_res &res) {
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
    // map a||b with c
    // key: a||b, value: {a||b||c, a||b||d...}
    map<string, multiset<string>> map_abc;
    for (const auto &mm: param.MM) {
        string a, b, c;
        if (find_w123_from_w(mm.first, a, b, c) != 0) {
            cout << "[MLEMM_Setup] call find_w123_from_w err: " << mm.first << endl;
            return -1;
        }
        string ab = a + "∩" + b;
        if (map_abc.find(ab) == map_abc.end()) {
            multiset<string> abc;
            abc.insert(c);
            map_abc[ab] = abc;
        } else {
            map_abc[ab].insert(c);
        }
    }
    // init mmt and dx
    map<string, cdc> MMt;
    map<string, int> ZX;
    for (const auto &abc: map_abc) {
        // get a b
        string a, b;
        if (find_w12_from_w(abc.first, a, b) != 0) {
            cout << "[MLEMM_Setup] call find_w123_from_w err: " << abc.first << endl;
            return -1;
        }
        for (const auto &c: abc.second) {
            string act_abc = abc.first + "∩" + c;
            vector<string> v_abc = param.MM[act_abc];
            // keyt_ab = F(Kt, a||b)
            string key_ab_t = H1(kt, a + b);
            // keyabc_enc=F(Kp, a||b||c)
            string key_abc_enc = H1(res.kp, a + b + c);
            // keyabc_x=F(Kp, a||b||c)
            string key_abc_x = H1(res.kx, a + b + c);

            vector<string> etag(v_abc.size());
            vector<string> ev(v_abc.size());
            for (int i = 0; i < v_abc.size(); i++) {
                // tag = F(ka_t, v)
                string tag = H1(key_ab_t, v_abc[i]);
                // etag = Enc(keyab_enc, tag)
                encrypt(key_abc_enc, tag, etag[i]);
                // ev = Enc(kenc, v)
                encrypt(res.kenc, v_abc[i], ev[i]);
                // X <- dtag = F(keyabc_x, tag)
                res.X.insert(H1(key_abc_x, tag));
            }
            MMt[act_abc] = cdc(etag, ev);
        }
    }
    // mskt, EMMp = sEMM_Setup(1_lamda, MMp)
    sEMM_Setup(param.lambda, MMt, res.mskp, res.EMMp);
    return 0;
}

int MLEMM_Token(mlemm_token_param &param, mlemm_token_res &res) {
    //clear kx
    res.kx.clear();
    if (param.words.size() < 3) {
        cout << "[MLEMM_Token] words size too little err: " << param.words.size() << endl;
        return -1;
    }
    // tokp1=sEMM.Token(mskp, (w1, w2, w3))
    sEMM_Token_w3(param.mskp, param.words[0], param.words[1], param.words[2],res.tokp);
    // k_w12_enc = hash(kp, w1+w2+w3)
    res.k_l123_enc = H1(param.kp, param.words[0] + param.words[1] + param.words[2]);
    for (int i = 3; i < param.words.size(); i++) {
        // kx = F(kx, w1+w2+wd)
        string kx = H1(param.kx, param.words[0] + param.words[1] + param.words[i]);
        res.kx.push_back(kx);
    }
    // done
    return 0;
}

int MLEMM_Search(mlemm_search_param &param, vector<string> &ev) {
    // clear ans
    ev.clear();
    // tagl = sEMM.Search(tokp, EMMp)
    cdc tags_l;
    sEMM_Search(param.tokp, param.EMMp, tags_l);
    // tagi = Dec(k_l12_enc, etagl)
    vector<string> tag(tags_l.c.size());
    for (int i = 0; i < tags_l.c.size(); i++) {
        decrypt(param.k_l123_enc, tags_l.c[i], tag[i]);
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

int MLEMM_Resolve(mlemm_resolve_param &param, vector<string> &ans) {
    for (const auto &ev: param.ev) {
        string a;
        decrypt(param.kenc, ev, a);
        ans.push_back(a);
    }
    // done
    return 0;
}