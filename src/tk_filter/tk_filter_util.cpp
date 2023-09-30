#include <tk_filter/tk_filter_util.h>

int find_w12_from_w(string word, string &w1, string &w2) {
    if (word == "") {
        w1 = "";
        w2 = "";
        return 0;
    }
    int idx = word.find("∩");
    if (idx == -1) {
        cout << "substr without ∩, wrong msg: " << word << endl;
        return -1;
    }
    w1 = word.substr(0, idx);
    w2 = word.substr(idx + 3, word.size() - idx);
    return 0;
}

string Permutation(int n, vector<int> kep, string pin) {
    string pout = "";
    for (size_t i = 0; i < n; i++) {
        pout += (pin[kep[i]]);
    }
    return pout;
}

string De_Permutation(int n, vector<int> kep, string dpin) {
    char dpout[n];
    for (size_t i = 0; i < n; i++) {
        dpout[kep[i]] = dpin[i];
    }
    string ret = "";
    for (int i = 0; i < n; i++) {
        ret += dpout[i];
    }
    return ret;
}

int Permutation2(int start, vector<int> kep, vector<string> pin, vector<string> &dpin) {
    for (size_t i = 0; i < kep.size(); i++) {
        dpin[i + start] = (pin[start + kep[i]]);
    }
    return 0;
}

int De_Permutation2(int start, vector<int> kep, vector<string> dpin, vector<string> &pin) {
    for (size_t i = 0; i < kep.size(); i++) {
        pin[kep[i] + start] = dpin[i + start];
    }
    return 0;
}

vector<int> Find_CK(int n, vector<int> kpa, vector<int> kpb) {
    map<int, int> ck_mpa;
    for (int i = 0; i < n; i++) {
        ck_mpa.insert(pair<int, int>(kpa[i], i));
    }
    vector<int> ret(n);
    for (int i = 0; i < n; i++) {
        ret[i] = ck_mpa[kpb[i]];
    }
    return ret;
}

int Permutationkey_Gen(string key, int n, vector<int> &ret) {
    if (ret.size() != n) {
        cout << "[Permutationkey_Gen] ret size is not n: " << ret.size() << endl;
        return -1;
    }
    for (int i = 0; i < n; i++) {
        ret[i] = i;
    }
    unsigned int tmp, tempo;
    for (int i = 0; i < n; i++) {
        string iTostr = to_string(i);
        tmp = hash_k_int(iTostr, key);
        tmp = tmp % n;
        tempo = ret[tmp];
        ret[tmp] = ret[i];
        ret[i] = tempo;
    }
    return 0;
}

int AONTH(int ctr, string m, string &mplus, string &dmplus) {
    int m_size = m.size();
    string keyp = H1(m, ANOTHKEY).substr(0, m_size);
    keyp = padding(keyp, m_size);

    // x = m ^ Hash(ctr+1, k')
    string ctrPlus = to_string(ctr + 1);
    string digest = H1(ctrPlus, keyp).substr(0, m_size);
    string x = Xor(m, digest);

    // dm' = k' ^ H(x)
    string hash_x = H(x).substr(0, m_size);
    dmplus = Xor(keyp, hash_x);
    dmplus = padding(dmplus, m_size);

    // m' = x ^ H(dm' ^ (ctr+1))
    string pad_ctr = padding(ctrPlus, m_size);
    string xor_param = H(Xor(dmplus, pad_ctr)).substr(0, m_size);
    mplus = Xor(x, xor_param);
    if (m.size() != mplus.size()) {
        cout << "[AONTH] m size is not equal to mplus: " << m.size() << "," << mplus.size() << endl;
        return -1;
    }
    if (m.size() != dmplus.size()) {
        cout << "[AONTH] m size is not equal to dmplus: " << m.size() << "," << dmplus.size() << endl;
        return -1;
    }
    return 0;
}

int D_AONTH(int ctr, string mplus, string dmplus, string &m) {
    if (mplus.size() != dmplus.size()) {
        cout << "[D_AONTH] mplus size is not equal to dmplus: " << mplus.size() << "," << dmplus.size()
                  << endl;
        return -1;
    }
    int m_size = mplus.size();
    // x = m' ^ H(dm' ^ (ctr+1))
    string ctrPlus = to_string(ctr + 1);
    string pad_ctr = padding(ctrPlus, m_size);
    string xor_param = H(Xor(dmplus, pad_ctr)).substr(0, m_size);
    string x = Xor(mplus, xor_param);

    // k' = dm' ^ H(x)
    string hash_x = H(x).substr(0, m_size);
    string keyp = Xor(dmplus, hash_x);

    // m = x ^ Hash(ctr+1, k')
    string digest = H1(ctrPlus, keyp).substr(0, m_size);
    m = Xor(x, digest);
    if (m.size() != mplus.size()) {
        cout << "[D_AONTH] m size is not right: " << m.size() << "," << mplus.size() << endl;
        return -1;
    }
    return 0;
}

int TK_Gen(vector<string> &key, vector<string> &w, int len, int doc,
           vector<int> &P1, vector<int> &P2, vector<int> &P3, string &keyphi) {
    if (key.size() != 3 || w.size() != 2) {
        cout << "[TK_Gen] wrong input, the lenght of key should be 3 and w should be 2" << endl;
        return -1;
    }
    if (P1.size() != len || P2.size() != len || P3.size() != doc) {
        cout << "[TK_Gen] the lenght of P1 and P2 is not len, and P3 is not n" << endl;
        return -1;
    }
    string keyw1 = H1(key[0], w[0]);
    string keyw2 = H1(key[0], w[1]);
    string key3 = H1(key[1], w[0] + w[1]);
    keyphi = H1(key[2], w[0] + w[1]);

    Permutationkey_Gen(keyw1, len, P1);
    Permutationkey_Gen(keyw2, len, P2);
    Permutationkey_Gen(key3, doc, P3);
    return 0;
}

int TK_Enc(vector<string> &key, vector<string> &w, vector<string> &m, int len,
           map<string, int> &ZX,
           vector<string> &c, vector<string> &dc, map<string, int> &DX) {
    if (m.size() == 0) {
        return 0;
    }
    if (m.size() != c.size() || c.size() != dc.size()) {
        cout << "[TK_Enc] wrong input, len(m) != len(c) != len(dc)" << endl;
        return -1;
    }
    // call TK-Gen(k1, k2, kphi, w1, w2, len, 2)
    vector<int> P1(len);
    vector<int> P2(len);
    vector<int> P3(2);
    string keyphi;
    if (TK_Gen(key, w, len, 2, P1, P2, P3, keyphi) != 0) {
        cout << "[TK_Enc] run TK_Gen err" << endl;
        return -1;
    }
    for (int i = 0; i < m.size(); i++) {
        int ctr = ZX[m[i]];
        // (mi', dmi') = AONTH(mi, ctr)
        string mplus, dmplus;
        if (AONTH(ctr, m[i], mplus, dmplus) != 0) {
            cout << "[TK_Enc] run AONTH err" << endl;
            return -1;
        }
        // DX[(a,b)]<-ctr
        DX.insert(pair<string, int>{mplus + dmplus, ctr});
        // (mi', dmi')= Perm(P3, (mi', dmi'))
        vector<string> pin{mplus, dmplus};
        vector<string> dpin(2);
        Permutation2(0, P3, pin, dpin);
        mplus = dpin[0];
        dmplus = dpin[1];
        // ci = Perm(Pi, mi') ^ Perm(P2, kphi)
        c[i] = Xor(Permutation(P1.size(), P1, mplus), Permutation(P2.size(), P2, keyphi));
        // dci = Perm(Pi, dmi') ^ Perm(P2, ci)
        dc[i] = Xor(Permutation(P1.size(), P1, dmplus), Permutation(P2.size(), P2, c[i]));
    }
    return 0;
}

int TK_Dec(vector<string> &key, vector<string> &w, vector<string> &c,
           vector<string> &dc,
           int len, map<string, int> &DX, vector<string> &m) {
    if (c.size() == 0 && dc.size() == 0) {
        return 0;
    }
    if (m.size() != c.size() || c.size() != dc.size()) {
        cout << "[TK_Dec] wrong input, len(m) != len(c) != len(dc)" << endl;
        return -1;
    }
    // call TK-Gen(k1, k2, kphi, w1, w2, len, 2)
    vector<int> P1(len);
    vector<int> P2(len);
    vector<int> P3(2);
    string keyphi;
    if (TK_Gen(key, w, len, 2, P1, P2, P3, keyphi) != 0) {
        cout << "[TK_Dec] run TK_Gen err" << endl;
        return -1;
    }
    for (int i = 0; i < c.size(); i++) {
        // dmi' = DePerm(Pi, dci ^ Perm(P2, ci))
        string dmplus = De_Permutation(P1.size(), P1, Xor(dc[i], Permutation(P2.size(), P2, c[i])));
        // mi' = DePerm(Pi, ci ^ Perm(P2, kphi))
        string mplus = De_Permutation(P1.size(), P1, Xor(c[i], Permutation(P2.size(), P2, keyphi)));
        // (mi', dmi')= DePerm(P3, (mi', dmi'))
        vector<string> dpin{mplus, dmplus};
        vector<string> pin(2);
        De_Permutation2(0, P3, dpin, pin);
        mplus = pin[0];
        dmplus = pin[1];
        // find from dx
        int ctr = DX[mplus + dmplus];
        // mi = D-AONTH(ctr, mi', dmi')
        if (D_AONTH(ctr, mplus, dmplus, m[i]) != 0) {
            cout << "[TK_Dec] run D_AONTH err" << endl;
            return -1;
        }
    }
    return 0;
}

int TK_ReGen(vector<string> &key, vector<string> &w, int len,
             vector<int> &RetCK3, vector<vector<int>> &RetP2, vector<string> &RetKeyPhi) {
    if (key.size() != 3 || w.size() != 3) {
        cout << "[TK_ReGen] wrong input, the lenght of key should be 3 and w should be 3" << endl;
        return -1;
    }
    if (RetP2.size() != 2 || RetKeyPhi.size() != 2) {
        cout << "[TK_ReGen] wrong output, the lenght of P2 and keypai should be 2 " << endl;
        return -1;
    }
    vector<int> P1(len);
    vector<int> P2(len);
    vector<int> P3(2);
    string keyphi;
    vector<string> w_input(2);
    w_input[0] = w[0];
    w_input[1] = w[1];
    // call TK-Gen
    if (TK_Gen(key, w_input, len, 2, P1, P2, P3, keyphi) == -1) {
        cout << "[TK_ReGen] run TK_Gen err" << endl;
        return -1;
    }

    vector<int> P1plus(len);
    vector<int> P2plus(len);
    vector<int> P3plus(2);
    string keyphiplus;
    w_input[1] = w[2];
    // call TK-Gen
    if (TK_Gen(key, w_input, len, 2, P1plus, P2plus, P3plus, keyphiplus) == -1) {
        cout << "[TK_ReGen] run TK_Gen err" << endl;
        return -1;
    }

    RetCK3 = Find_CK(2, P3, P3plus); // CK3=FindCk(P3, P3')
    RetP2[0] = P2;                   // P2
    RetP2[1] = P2plus;               // P2'
    RetKeyPhi[0] = keyphi;           // keyfai
    RetKeyPhi[1] = keyphiplus;       // keyfai'
    return 0;
}

int TK_ReEnc(vector<int> &CK3, vector<vector<int>> &P2, vector<string> &KeyPhi,
             vector<string> &c, vector<string> &dc,
             vector<string> &cplus, vector<string> &dcplus) {
    if (P2.size() != 2 || KeyPhi.size() != 2) {
        cout << "[TK_ReEnc] wrong output, the lenght of P2 should be 2, KeyPhi should be 2, P2 size: " << P2.size()
                  << ", KeyPhi size: " << KeyPhi.size() << endl;
        return -1;
    }
    if (c.size() != cplus.size() || dc.size() != dcplus.size() || c.size() != dc.size()) {
        cout << "[TK_ReEnc] the length of c, dc, cplus and dcplus should be equal" << endl;
        return -1;
    }
    for (int i = 0; i < c.size(); i++) {
        // dci' = dci ^ Perm(P2, ci)
        dcplus[i] = Xor(dc[i], Permutation(P2[0].size(), P2[0], c[i]));
        // ci' = ci ^ Perm(P2, KeyPhi)
        cplus[i] = Xor(c[i], Permutation(P2[0].size(), P2[0], KeyPhi[0]));
        // (ci', dci') = Perm(CK3, (ci', dci'))
        vector<string> pin{cplus[i], dcplus[i]};
        vector<string> dpin(2);
        Permutation2(0, CK3, pin, dpin);
        cplus[i] = dpin[0];
        dcplus[i] = dpin[1];
        // ci'= ci' ^ Perm(P2', keyphi')
        cplus[i] = Xor(cplus[i], Permutation(P2[1].size(), P2[1], KeyPhi[1]));
        // dci'= dci' ^ Perm(P2', ci-1')
        dcplus[i] = Xor(dcplus[i], Permutation(P2[1].size(), P2[1], cplus[i]));
    }
    return 0;
}
