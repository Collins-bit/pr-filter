#include <rpc/server.h>

std::map<std::string, cdc> EMMt;
std::multiset<std::string> Xset;

std::vector<std::string> strChangeToVec(google::protobuf::RepeatedPtrField<std::string> value) {
    std::vector<std::string> vec(value.size());
    for (int i = 0; i < value.size(); i++) {
        vec[i] = value[i];
    }
    return vec;
}

std::vector<int> intChangeToVec(google::protobuf::RepeatedField<int32_t> value) {
    std::vector<int> vec(value.size());
    for (int i = 0; i < value.size(); i++) {
        vec[i] = value[i];
    }
    return vec;
}

Status PrFilterServiceImpl::SetupEmmt(ServerContext *context,
                                      ServerReader<SetupEmmtRequest> *reader, google::protobuf::Empty *) {
    // clear map
    EMMt.clear();
    // init EMMT
    SetupEmmtRequest emmt;
    while (reader->Read(&emmt)) {
        const auto &key = emmt.key();
        std::vector<std::string> c_vec = strChangeToVec(emmt.value().c());
        std::vector<std::string> dc_vec = strChangeToVec(emmt.value().dc());
        EMMt[key] = cdc(c_vec, dc_vec);
    }
    std::cout << "upsert Emmt success!" << std::endl;
    return Status::OK;
}

Status PrFilterServiceImpl::SetupXset(ServerContext *context,
                                      ServerReader<SetupXsetRequest> *reader, google::protobuf::Empty *) {
    // clear multiset
    Xset.clear();
    // init Xset
    SetupXsetRequest xset;
    while (reader->Read(&xset)) {
        const auto &xset_v = xset.xset_v();
        Xset.insert(xset_v);
    }
    std::cout << "upsert Xset success!" << std::endl;
    return Status::OK;
}

Status PrFilterServiceImpl::Search(ServerContext *context,
                                   const SearchRequest *request, SearchResponse *searchResponse) {
    // tokp_vec
    std::vector<key_re_d> tokp_vec(request->tokp_vec().size());
    for (int i = 0; i < tokp_vec.size(); i++) {
        tokp_vec[i].kx = request->tokp_vec()[i].kx();
        tokp_vec[i].CK = intChangeToVec(request->tokp_vec()[i].ck());
        std::vector<std::vector<int>> P2(2);
        P2[0] = intChangeToVec(request->tokp_vec()[i].p2());
        P2[1] = intChangeToVec(request->tokp_vec()[i].p2_plus());
        tokp_vec[i].P2 = P2;
        std::vector<std::string> KeyPhi(2);
        KeyPhi[0] = request->tokp_vec()[i].key_phi();
        KeyPhi[1] = request->tokp_vec()[i].key_phi_plus();
        tokp_vec[i].KeyPhi = KeyPhi;
    }
    // emm
    EMM emm;
    emm.EMMt = EMMt;
    emm.Xset = Xset;
    // search_param
    pr_filter_search_param search_param;
    search_param.tokp = request->tokp();
    search_param.k_w12_enc = request->k_w12_enc();
    search_param.tokp_vec = tokp_vec;
    search_param.emm = emm;
    pr_filter_search_res search_res;
    std::cout << "search in server success!" << std::endl;
    // call PR_Filter_Search
#ifdef TEST
    TimeUtil time_util;
    time_util.initTime();
    for (int i = 0; i < 1000; i++) {
        if (PR_Filter_Search(search_param, search_res) != 0) {
            return Status::CANCELLED;
        }
    }
    time_util.endTime("search process", 1000);
#else
    if (PR_Filter_Search(search_param, search_res) != 0) {
        return Status::CANCELLED;
    }
#endif
    // respone
    for (auto &c: search_res.c) {
        searchResponse->add_c(c);
    }
    for (auto &dc: search_res.dc) {
        searchResponse->add_dc(dc);
    }
    return Status::OK;
}

