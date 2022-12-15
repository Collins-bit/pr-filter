#include <algorithm/pr_filter_server.h>

void sEMM_Search(std::string tokp, std::map<std::string, cdc> EMMp, cdc &tags)
{
    tags = EMMp[tokp];
}

int PR_Filter_Search(pr_filter_search_param param, pr_filter_search_res &res)
{
    // tagl = sEMM.Search(tokp, EMMp)
    cdc tags_l;
    sEMM_Search(param.tokp, param.emm.EMMt, tags_l);
    // ci = Dec(k_w1_w2_enc, tagl)
    std::vector<std::string> c(tags_l.size());
    std::vector<std::string> dc(tags_l.size());
    for (int i = 0; i < tags_l.size(); i++)
    {
        decrypt(param.k_w12_enc, tags_l.c[i], c[i]);
        decrypt(param.k_w12_enc, tags_l.dc[i], dc[i]);
    }
    // optimization
    // for l=1..L
    // for d=3..nl
    // cl = PR.ReEnc(k_re_d, cl)
    // dtage(l,d) = F(kx, cl')
    int s = c.size();
    std::vector<bool> vaild(s, true);
    std::vector<std::string> cplus(s);
    std::vector<std::string> dcplus(s);
    std::string dtag1, dtag2;
    for (int i = 0; i < param.tokp_vec.size(); i++)
    {
        if (Pr_ReEnc(param.tokp_vec[i].CK, param.tokp_vec[i].P2, param.tokp_vec[i].KeyPhi, c, dc, cplus, dcplus) != 0)
        {
            std::cout << "[PR_Filter_Search] call Pr_ReEnc failed" << std::endl;
            return -1;
        }
        for (int j = 0; j < s; j++)
        {
            c[j] = cplus[j];
            dc[j] = dcplus[j];
        }
        for (int j = 0; j < s; j++)
        {
            if (!vaild[j])
                continue;
            dtag1 = H1(param.tokp_vec[i].kx, cplus[j]);
            dtag2 = H1(param.tokp_vec[i].kx, dcplus[j]);
            if (param.emm.Xset.find(dtag1+dtag2) == param.emm.Xset.end())
            {
                vaild[j] = false;
            }
        }
    }
    res.c = cplus;
    res.dc = dcplus;
    res.vaild = vaild;
    // done
    return 0;
}