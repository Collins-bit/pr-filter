#include <rpc/client.h>

int TKFilterClient::SendEmmt(const std::map<std::string, std::vector<c_ectr>> &EMMt) {
    ClientContext context;
    std::unique_ptr<grpc::ClientWriter<SetupEmmtRequest> > writer;
    SetupEmmtRequest request;
    google::protobuf::Empty response;
    writer = stub_->SetupEmmt(&context, &response);
    for (const auto &emmt: EMMt) {
        request.set_key(emmt.first);
        for (const auto &i: emmt.second) {
            auto *value = request.add_value();
            value->set_c(i.c);
            value->set_ectr(i.ectr);
        }
        if (!writer->Write(request)) {
            std::cout << "send emmt failed, the stream has been closed" << std::endl;
            break;
        }
        request.clear_key();
        request.clear_value();
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

int TKFilterClient::SendXset(const std::multiset<std::string> &Xset) {
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

int TKFilterClient::SearchInServer(tk_filter_token_res token_res, std::vector<c_ectr> &ct) {
    ClientContext context;
    SearchRequest request;
    SearchResponse response;
    request.set_tokp(token_res.tokp);
    for (const auto& ck_i: token_res.ck) {
        auto *cks = request.add_ck();
        for(const auto& i: ck_i){
            cks->add_ck(i);
        }
    }
    Status status = stub_->Search(&context, request, &response);

    if (!status.ok()) {
        std::cout << "search failed: " << status.error_code() << ", result: " << status.error_message() << std::endl;
        return -1;
    }
    for (const auto &c_val: response.ct()) {
        auto cectr = c_ectr(c_val.c(), c_val.ectr());
        ct.push_back(cectr);
    }
    return 0;
}