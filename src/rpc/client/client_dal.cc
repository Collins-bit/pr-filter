#include <rpc/client.h>

int PrFilterClient::SendEmmt(const std::map<std::string, cdc> &EMMt) {
    ClientContext context;
    std::unique_ptr<grpc::ClientWriter<SetupEmmtRequest> > writer;
    SetupEmmtRequest request;
    google::protobuf::Empty response;
    writer = stub_->SetupEmmt(&context, &response);
    for (const auto &emmt: EMMt) {
        request.set_key(emmt.first);
        auto *value = new PrFilter::cbc();
        for (const auto &i: emmt.second.c) {
            value->add_c(i);
        }
        for (const auto &i: emmt.second.dc) {
            value->add_dc(i);
        }
        request.set_allocated_value(value);
        if (!writer->Write(request)) {
            std::cout << "send emmt failed, the stream has been closed" << std::endl;
            break;
        }
    }
    writer->WritesDone();
    grpc::Status status = writer->Finish();
    if (status.ok()) {
        return 0;
    } else {
        std::cout << "send emmt failed: " << status.error_code() << ", result: " << status.error_message() << std::endl;
        return -1;
    }
}

int PrFilterClient::SendXset(const std::multiset<std::string> &Xset) {
    ClientContext context;
    SetupXsetRequest request;
    google::protobuf::Empty response;
    std::unique_ptr<grpc::ClientWriter<SetupXsetRequest> > writer;
    writer = stub_->SetupXset(&context, &response);
    for (const auto &xset_v: Xset) {
        request.set_xset_v(xset_v);
        if (!writer->Write(request)) {
            std::cout << "send xset failed, the stream has been closed" << std::endl;
            break;
        }
    }
    writer->WritesDone();
    grpc::Status status = writer->Finish();
    if (status.ok()) {
        return 0;
    } else {
        std::cout << "send xset failed: " << status.error_code() << ", result: " << status.error_message() << std::endl;
        return -1;
    }
}

int PrFilterClient::SearchInServer(pr_filter_token_res token_res, std::vector<std::string> &c,
                                   std::vector<std::string> &dc, std::vector<bool> &vaild) {
    ClientContext context;
    SearchRequest request;
    SearchResponse response;
    request.set_tokp(token_res.tokp);
    request.set_k_w12_enc(token_res.k_w12_enc);
    for (auto tp: token_res.tokp_vec) {
        auto *key_re_d = request.add_tokp_vec();
        key_re_d->set_kx(tp.kx);
        for (int ck_val: tp.CK) {
            key_re_d->add_ck(ck_val);
        }
        for (int p2_val: tp.P2[0]) {
            key_re_d->add_p2(p2_val);
        }
        for (int p2_plus_val: tp.P2[1]) {
            key_re_d->add_p2_plus(p2_plus_val);
        }
        key_re_d->set_key_phi(tp.KeyPhi[0]);
        key_re_d->set_key_phi_plus(tp.KeyPhi[1]);
    }
    Status status = stub_->Search(&context, request, &response);

    if (!status.ok()) {
        std::cout << "search failed: " << status.error_code() << ", result: " << status.error_message() << std::endl;
        return -1;
    }
    for (const auto &c_val: response.c()) {
        c.push_back(c_val);
    }
    for (const auto &dc_val: response.dc()) {
        dc.push_back(dc_val);
    }
    return 0;
}