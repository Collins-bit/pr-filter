#include <conj_filter/conj_filter.h>

int Conj_Filter_Setup(conj_filter_setup_param &param, conj_filter_setup_res &res) {
    // clear EMM and DX
    res.EMMp.clear();
    res.X.clear();
    // init rand
    srand(time(nullptr));
    // generate random key
    res.kp = Gen_RandKey(param.lambda);
    std::string kt = Gen_RandKey(param.lambda);
    res.kx = Gen_RandKey(param.lambda);
    res.kenc = Gen_RandKey(param.lambda);
    // init mmt and dx
    std::map<std::string, cdc> MMt;
    std::map<std::string, int> ZX;
    for (auto mm: param.MM) {
        // get a b
        std::string a, b;
        if (find_w12_from_w(mm.first, a, b) != 0) {
            std::cout << "[Conj_Filter_Setup] call find_w12_from_w err: " << mm.first << std::endl;
            return -1;
        }
        // keyt_a = F(Kt, a)
        std::string key_a_t = H1(kt, a);
        // keyab_enc=F(Kp, a||b)
        std::string key_ab_enc = H1(res.kp, a + b);
        // keyab_x=F(Kp, a||b)
        std::string key_ab_x = H1(res.kx, a + b);

        std::vector<std::string> etag(mm.second.size());
        std::vector<std::string> ev(mm.second.size());
        for (int i = 0; i < mm.second.size(); i++) {
            // tag = F(ka_t, v)
            std::string tag = H1(key_a_t, mm.second[i]);
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
    sEMM_Setup(param.lambda, MMt, res.mskp, res.EMMp);
    return 0;
}

int Conj_Filter_Token(conj_filter_token_param &param, conj_filter_token_res &res) {
    //clear kx
    res.kx.clear();
    if (param.words.size() < 2) {
        std::cout << "[Conj_Filter_Token] words size too little err: " << param.words.size() << std::endl;
        return -1;
    }
    // tokp1=sEMM.Token(mskp, (w1, w2))
    sEMM_Token(param.mskp, param.words[0], param.words[1], res.tokp);
    // k_w12_enc = hash(kp, w1+w2)
    res.k_l12_enc = H1(param.kp, param.words[0] + param.words[1]);
    for (int i = 2; i < param.words.size(); i++) {
        // kx = F(kx, w1+wd)
        std::string kx = H1(param.kx, param.words[0] + param.words[i]);
        res.kx.push_back(kx);
    }
    // done
    return 0;
}

int Conj_Filter_Search(conj_filter_search_param &param, std::vector<std::string> &ev) {
    // clear ans
    ev.clear();
    // tagl = sEMM.Search(tokp, EMMp)
    cdc tags_l;
    sEMM_Search(param.tokp, param.EMMp, tags_l);
    // tagi = Dec(k_l12_enc, etagl)
    std::vector<std::string> tag(tags_l.c.size());
    for (int i = 0; i < tags_l.c.size(); i++) {
        decrypt(param.k_l12_enc, tags_l.c[i], tag[i]);
    }
    // dtag(l,d) = F(K_x_l, tagd)
    for (int l = 0; l < tag.size(); l++) {
        bool flag = true;
        for (int d = 0; d < param.kx.size(); d++) {
            std::string dtag = H1(param.kx[d], tag[l]);
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

int Conj_Filter_Resolve(conj_filter_resolve_param &param, std::vector<std::string> &ans) {
    for (const auto &ev: param.ev) {
        std::string a;
        decrypt(param.kenc, ev, a);
        ans.push_back(a);
    }
    // done
    return 0;
}