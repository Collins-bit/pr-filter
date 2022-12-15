#include <algorithm/pr_filter_util.h>

int find_w12_from_w(std::string word, std::string &w1, std::string &w2)
{
    if (word == "")
    {
        w1 = "";
        w2 = "";
        return 0;
    }
    int idx = word.find("∩");
    if (idx == -1)
    {
        std::cout << "substr without ∩, wrong msg: " << word << std::endl;
        return -1;
    }
    w1 = word.substr(0, idx);
    w2 = word.substr(idx + 3, word.size() - idx);
    return 0;
}

// 排列加密算法: 将pin按kep顺序排列，pin为string
std::string Permutation(int n, std::vector<int> kep, std::string pin)
{
    std::string pout = "";
    for (size_t i = 0; i < n; i++)
    {
        pout += (pin[kep[i]]);
    }
    return pout;
}

// 排列加密算法: 将pin按kep顺序重排回原列
std::string De_Permutation(int n, std::vector<int> kep, std::string dpin)
{
    char dpout[n];
    for (size_t i = 0; i < n; i++)
    {
        dpout[kep[i]] = dpin[i];
    }
    std::string ret = "";
    for (int i = 0; i < n; i++)
    {
        ret += dpout[i];
    }
    return ret;
}

// 排列加密算法：将pin按kep顺序排列，pin为string数组，start为开始位置
int Permutation2(int start, std::vector<int> kep, std::vector<std::string> pin, std::vector<std::string> &dpin)
{
    for (size_t i = 0; i < kep.size(); i++)
    {
        dpin[i + start] = (pin[start + kep[i]]);
    }
    return 0;
}

// 排列加密算法: 将pin按kep顺序重排回原列
int De_Permutation2(int start, std::vector<int> kep, std::vector<std::string> dpin, std::vector<std::string> &pin)
{
    for (size_t i = 0; i < kep.size(); i++)
    {
        pin[kep[i] + start] = dpin[i + start];
    }
    return 0;
}

// 根据kpa和kpb调整顺序
std::vector<int> Find_CK(int n, std::vector<int> kpa, std::vector<int> kpb)
{
    std::map<int, int> ck_mpa;
    for (int i = 0; i < n; i++)
    {
        ck_mpa.insert(std::pair<int, int>(kpa[i], i));
    }
    std::vector<int> ret(n);
    for (int i = 0; i < n; i++)
    {
        ret[i] = ck_mpa[kpb[i]];
    }
    return ret;
}

// 密钥扩展函数，将密钥从key长度扩展到n长度
int Permutationkey_Gen(std::string key, int n, std::vector<int> &ret)
{
    if (ret.size() != n)
    {
        std::cout << "[Permutationkey_Gen] ret size is not n: " << ret.size() << std::endl;
        return -1;
    }
    for (int i = 0; i < n; i++)
    {
        ret[i] = i;
    }
    unsigned int tmp, tempo;
    for (int i = 0; i < n; i++)
    {
        std::string iTostr = toBinary(i);
        tmp = hash_k_int(iTostr, key);
        tmp = tmp % n;
        tempo = ret[tmp];
        ret[tmp] = ret[i];
        ret[i] = tempo;
    }
    return 0;
}

int AONTH(int ctr, std::string m, std::string &mplus, std::string &dmplus)
{
    int m_size = m.size();
    std::string keyp = H1(m, ANOTHKEY).substr(0, m_size);
    keyp = padding(keyp, m_size);

    // x = m ^ Hash(ctr+1, k')
    std::string ctrPlus = std::to_string(ctr + 1);
    std::string digest = H1(ctrPlus, keyp).substr(0, m_size);
    std::string x = Xor(m, digest);

    // dm' = k' ^ H(x)
    std::string hash_x = H(x).substr(0, m_size);
    dmplus = Xor(keyp, hash_x);
    dmplus = padding(dmplus, m_size);

    // m' = x ^ H(dm' ^ (ctr+1))
    std::string pad_ctr = padding(ctrPlus, m_size);
    std::string xor_param = H(Xor(dmplus, pad_ctr)).substr(0, m_size);
    mplus = Xor(x, xor_param);
    if (m.length() != mplus.length())
    {
        std::cout << "[AONTH] m size is not equal to mplus: " << m.length() << "," << mplus.size() << std::endl;
        return -1;
    }
    if (m.length() != dmplus.length())
    {
        std::cout << "[AONTH] m size is not equal to dmplus: " << m.length() << "," << dmplus.size() << std::endl;
        return -1;
    }
    return 0;
}

int D_AONTH(int ctr, std::string mplus, std::string dmplus, std::string &m)
{
    if (mplus.length() != dmplus.length())
    {
        std::cout << "[D_AONTH] mplus size is not equal to dmplus: " << mplus.length() << "," << dmplus.size() << std::endl;
        return -1;
    }
    int m_size = mplus.size();
    // x = m' ^ H(dm' ^ (ctr+1))
    std::string ctrPlus = std::to_string(ctr + 1);
    std::string pad_ctr = padding(ctrPlus, m_size);
    std::string xor_param = H(Xor(dmplus, pad_ctr)).substr(0, m_size);
    std::string x = Xor(mplus, xor_param);

    // k' = dm' ^ H(x)
    std::string hash_x = H(x).substr(0, m_size);
    std::string keyp = Xor(dmplus, hash_x);

    // m = x ^ Hash(ctr+1, k')
    std::string digest = H1(ctrPlus, keyp).substr(0, m_size);
    m = Xor(x, digest);
    if (m.length() != mplus.length())
    {
        std::cout << "[D_AONTH] m size is not right: " << m.length() << "," << mplus.size() << std::endl;
        return -1;
    }
    return 0;
}

int Pr_Gen(std::vector<std::string> key, std::vector<std::string> w, int len, int doc,
           std::vector<int> &P1, std::vector<int> &P2, std::vector<int> &P3, std::string &keyphi)
{
    if (key.size() != 3 || w.size() != 2)
    {
        std::cout << "[Pr_Gen] wrong input, the lenght of key should be 3 and w should be 2" << std::endl;
        return -1;
    }
    if (P1.size() != len || P2.size() != len || P3.size() != doc)
    {
        std::cout << "[Pr_Gen] the lenght of P1 and P2 is not len, and P3 is not n" << std::endl;
        return -1;
    }
    std::string keyw1 = H1(key[0], w[0]);
    std::string keyw2 = H1(key[0], w[1]);
    std::string key3 = H1(key[1], w[0] + w[1]);
    keyphi = H1(key[2], w[0] + w[1]);

    Permutationkey_Gen(keyw1, len, P1);
    Permutationkey_Gen(keyw2, len, P2);
    Permutationkey_Gen(key3, doc, P3);
    return 0;
}

int Pr_Enc(std::vector<std::string> key, std::vector<std::string> w, std::vector<std::string> m, int len, std::map<std::string, int> ZX,
           std::vector<std::string> &c, std::vector<std::string> &dc, std::map<std::string, int> &DX)
{
    if (m.size() == 0)
    {
        return 0;
    }
    if (m.size() != c.size() || c.size() != dc.size())
    {
        std::cout << "[Pr_Enc] wrong input, len(m) != len(c) != len(dc)" << std::endl;
        return -1;
    }
    // call Pr-Gen(k1, k2, kphi, w1, w2, len, 2)
    std::vector<int> P1(len);
    std::vector<int> P2(len);
    std::vector<int> P3(2);
    std::string keyphi;
    if (Pr_Gen(key, w, len, 2, P1, P2, P3, keyphi) != 0)
    {
        std::cout << "[Pr_Enc] run Pr_Gen err" << std::endl;
        return -1;
    }
    for (int i = 0; i < m.size(); i++)
    {
        int ctr = ZX[m[i]];
        // (mi', dmi') = AONTH(mi, ctr)
        std::string mplus, dmplus;
        if (AONTH(ctr, m[i], mplus, dmplus) != 0)
        {
            std::cout << "[Pr_Enc] run AONTH err" << std::endl;
            return -1;
        }
        // DX[(a,b)]<-ctr
        DX.insert(std::pair<std::string, int>{mplus + dmplus, ctr});
        // (mi', dmi')= Perm(P3, (mi', dmi'))
        std::vector<std::string> pin{mplus, dmplus};
        std::vector<std::string> dpin(2);
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

int Pr_Dec(std::vector<std::string> key, std::vector<std::string> w, std::vector<std::string> c, std::vector<std::string> dc,
           int len, std::map<std::string, int> DX, std::vector<std::string> &m)
{
    if (c.size() == 0 && dc.size() == 0)
    {
        return 0;
    }
    if (m.size() != c.size() || c.size() != dc.size())
    {
        std::cout << "[Pr_Dec] wrong input, len(m) != len(c) != len(dc)" << std::endl;
        return -1;
    }
    // call Pr-Gen(k1, k2, kphi, w1, w2, len, 2)
    std::vector<int> P1(len);
    std::vector<int> P2(len);
    std::vector<int> P3(2);
    std::string keyphi;
    if (Pr_Gen(key, w, len, 2, P1, P2, P3, keyphi) != 0)
    {
        std::cout << "[Pr_Dec] run Pr_Gen err" << std::endl;
        return -1;
    }
    for (int i = 0; i < c.size(); i++)
    {
        // dmi' = DePerm(Pi, dci ^ Perm(P2, ci))
        std::string dmplus = De_Permutation(P1.size(), P1, Xor(dc[i], Permutation(P2.size(), P2, c[i])));
        // mi' = DePerm(Pi, ci ^ Perm(P2, kphi))
        std::string mplus = De_Permutation(P1.size(), P1, Xor(c[i], Permutation(P2.size(), P2, keyphi)));
        // (mi', dmi')= DePerm(P3, (mi', dmi'))
        std::vector<std::string> dpin{mplus, dmplus};
        std::vector<std::string> pin(2);
        De_Permutation2(0, P3, dpin, pin);
        mplus = pin[0];
        dmplus = pin[1];
        // find from dx
        int ctr = DX[mplus + dmplus];
        // mi = D-AONTH(ctr, mi', dmi')
        if (D_AONTH(ctr, mplus, dmplus, m[i]) != 0)
        {
            std::cout << "[Pr_Dec] run D_AONTH err" << std::endl;
            return -1;
        }
    }
    return 0;
}

int Pr_ReGen(std::vector<std::string> key, std::vector<std::string> w, int len,
             std::vector<int> &RetCK3, std::vector<std::vector<int>> &RetP2, std::vector<std::string> &RetKeyPhi)
{
    if (key.size() != 3 || w.size() != 3)
    {
        std::cout << "[Pr_ReGen] wrong input, the lenght of key should be 3 and w should be 3" << std::endl;
        return -1;
    }
    if (RetP2.size() != 2 || RetKeyPhi.size() != 2)
    {
        std::cout << "[Pr_ReGen] wrong output, the lenght of P2 and keypai should be 2 " << std::endl;
        return -1;
    }
    std::vector<int> P1(len);
    std::vector<int> P2(len);
    std::vector<int> P3(2);
    std::string keyphi;
    std::vector<std::string> w_input(2);
    w_input[0] = w[0];
    w_input[1] = w[1];
    // call Pr-Gen
    if (Pr_Gen(key, w_input, len, 2, P1, P2, P3, keyphi) == -1)
    {
        std::cout << "[Pr_ReGen] run Pr_Gen err" << std::endl;
        return -1;
    }

    std::vector<int> P1plus(len);
    std::vector<int> P2plus(len);
    std::vector<int> P3plus(2);
    std::string keyphiplus;
    w_input[1] = w[2];
    // call Pr-Gen
    if (Pr_Gen(key, w_input, len, 2, P1plus, P2plus, P3plus, keyphiplus) == -1)
    {
        std::cout << "[Pr_ReGen] run Pr_Gen err" << std::endl;
        return -1;
    }

    RetCK3 = Find_CK(2, P3, P3plus); // CK3=FindCk(P3, P3')
    RetP2[0] = P2;                   // P2
    RetP2[1] = P2plus;               // P2'
    RetKeyPhi[0] = keyphi;           // keyfai
    RetKeyPhi[1] = keyphiplus;       // keyfai'
    return 0;
}

int Pr_ReEnc(std::vector<int> CK3, std::vector<std::vector<int>> P2, std::vector<std::string> KeyPhi, std::vector<std::string> c, std::vector<std::string> dc,
             std::vector<std::string> &cplus, std::vector<std::string> &dcplus)
{
    if (P2.size() != 2 || KeyPhi.size() != 2)
    {
        std::cout << "[Pr_ReEnc] wrong output, the lenght of P2 should be 2, KeyPhi should be 2 " << std::endl;
        return -1;
    }
    if (c.size() != cplus.size() || dc.size() != dcplus.size() || c.size() != dc.size())
    {
        std::cout << "[Pr_ReEnc] the length of c, dc, cplus and dcplus should be equal" << std::endl;
        return -1;
    }
    for (int i = 0; i < c.size(); i++)
    {
        // dci' = dci ^ Perm(P2, ci)
        dcplus[i] = Xor(dc[i], Permutation(P2[0].size(), P2[0], c[i]));
        // ci' = ci ^ Perm(P2, KeyPhi)
        cplus[i] = Xor(c[i], Permutation(P2[0].size(), P2[0], KeyPhi[0]));
        // (ci', dci') = Perm(CK3, (ci', dci'))
        std::vector<std::string> pin{cplus[i], dcplus[i]};
        std::vector<std::string> dpin(2);
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
