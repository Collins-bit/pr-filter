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

int TK_Enc(string kt, string a, string b, string v, string &c) {
    int l = v.size();
    string key_ab_t = H1(kt, a + b);
    vector<int> p_ab(l);
    Permutationkey_Gen(key_ab_t, l, p_ab);
    string ctr = H1(a, v);
    string rnd;
    encrypt(kt, ctr, rnd);
    string v_plus = Xor(rnd, v).substr(0, v.size());
    c = Permutation(p_ab.size(), p_ab, v_plus);
    return 0;
}

int TK_Dec(string kt, string kp, string a, string b, string c, string ectr, string &v) {
    int l = c.size();
    string key_ab_t = H1(kt, a + b);
    vector<int> p_ab(l);
    Permutationkey_Gen(key_ab_t, l, p_ab);
    string ctr, rnd;
    decrypt(kp, ectr, ctr);
    encrypt(kt, ctr, rnd);
    string v_plus = De_Permutation(p_ab.size(), p_ab, c);
    v = Xor(v_plus, rnd).substr(0, c.size());
    return 0;
}

int TK_ReGen(string kt, string a, string b, string w, int l, vector<int> &ck_w) {
    if(ck_w.size()!=l){
        cout << "[TK_ReGen] the lenght of ck_w should equal to l, ck_w size: " << ck_w.size() << endl;
        return -1;
    }
    string k_ab_t = H1(kt, a+b);
    string k_aw_t = H1(kt, a+w);
    vector<int> p1(l);
    Permutationkey_Gen(k_ab_t, l, p1);
    vector<int> p2(l);
    Permutationkey_Gen(k_aw_t, l, p2);
    ck_w = Find_CK(l, p1, p2);
    return 0;
}

int TK_ReEnc(vector<int> &ck_w, string c, string &c_plus) {
    c_plus = Permutation(ck_w.size(), ck_w, c);
    return 0;
}
