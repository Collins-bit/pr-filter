#include "rpc/server.h"

std::map<string, std::vector<c_ectr>> EMMt;
std::multiset<std::string> Xset;

std::vector<c_ectr> cectrChangeToVec(const google::protobuf::RepeatedPtrField<TKFilter::c_ectr>& cectr_field) {
    std::vector<c_ectr> c_ectr_vec;
    for (const auto& c_ectr_i : cectr_field) {
        auto c = c_ectr(c_ectr_i.c(), c_ectr_i.ectr());
        c_ectr_vec.push_back(c);
    }
    return c_ectr_vec;
}

Status TKFilterServiceImpl::SetupEmmt(ServerContext *context,
                                      ServerReader<SetupEmmtRequest> *reader, google::protobuf::Empty *) {
    // clear map
    EMMt.clear();
    // init EMMT
    SetupEmmtRequest emmt;
    while (reader->Read(&emmt)) {
        const auto &key = emmt.key();
        std::vector<c_ectr> c_ectr_vec = cectrChangeToVec(emmt.value());
        EMMt[key] = c_ectr_vec;
    }
    std::cout << "upsert Emmt success!" << std::endl;
    return Status::OK;
}

Status TKFilterServiceImpl::SetupXset(ServerContext *context,
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

Status TKFilterServiceImpl::Search(ServerContext *context,
                                   const SearchRequest *request, SearchResponse *searchResponse) {
    // tokp_vec
    std::vector<std::vector<int>> ck(request->ck().size());
    for (int i = 0; i < request->ck().size(); i++) {
        vector<int> c(request->ck()[i].ck().size());
        for(int j=0; j<request->ck()[i].ck().size(); j++){
            c[j] = request->ck()[i].ck()[j];
        }
        ck[i] = c;
    }
    // emm
    EMM emm;
    emm.EMMt = EMMt;
    emm.X = Xset;
    // search_param
    tk_filter_search_param search_param;
    search_param.tokp = request->tokp();
    search_param.ck = ck;
    search_param.emm = emm;
    tk_filter_search_res search_res;
    std::cout << "search in server success!" << std::endl;
    // call TK_Filter_Search
#ifdef TEST
    TimeUtil time_util;
    time_util.initTime();
    for (int i = 0; i < 1000; i++) {
        if (TK_Filter_Search(search_param, search_res) != 0) {
            return Status::CANCELLED;
        }
    }
    time_util.endTime("search process", 1000);
#else
    if (TK_Filter_Search(search_param, search_res) != 0) {
        return Status::CANCELLED;
    }
#endif
    // respone
    for (auto &i: search_res.ct) {
        auto *c_ectr_i = searchResponse->add_ct();
        c_ectr_i->set_c(i.c);
        c_ectr_i->set_ectr(i.ectr);
    }
    return Status::OK;
}

