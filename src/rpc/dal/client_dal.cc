#include <rpc/client.h>
#include <utility>

int setup(PrFilterClient &client, std::vector<std::string> command, int &mm_len,
          pr_filter_setup_param &setup_param, pr_filter_setup_res &setup_res) {
    // clear EMM and DX
    setup_res.emm.EMMt.clear();
    setup_res.emm.Xset.clear();
    setup_res.DX.clear();
    // get from command
    std::string fpath = "../testData/test_MM.txt";
    int keylen = 32;
    for (int i = 1; i < command.size(); i += 2) {
        if ((command[i] == "-fpath" || command[i] == "-f") && i + 1 < command.size()) {
            fpath = command[i + 1];
        } else if ((command[i] == "-keylen" || command[i] == "-k") && i + 1 < command.size()) {
            keylen = stoi(command[i + 1]);
        } else {
            std::cout << "[setup] wrong command!" << std::endl;
            return -1;
        }
    }
    // get mm from txt
    std::map<std::string, std::vector<std::string>> MM;
    int max_mm_len;
    Get_MM_from_fpath(fpath, MM, max_mm_len);
    Deal_mm_same_length(max_mm_len, MM);
    mm_len = max_mm_len;
    // init setup_param
    setup_param.MM = MM;
    setup_param.lambda = keylen;
    setup_param.mu = keylen;
    // call PR_Filter_Setup
    TimeUtil time_util;
    time_util.initTime();
    if (PR_Filter_Setup(setup_param, setup_res) != 0) {
        std::cout << "[setup] call PR_Filter_Setup failed!" << std::endl;
        return -2;
    }
    time_util.endTime("setup process");
    emmtChange2File(setup_res.emm.EMMt, "./emmt.txt");
    xsetChange2File(setup_res.emm.Xset, "./xset.txt");
    // send to server
    if (client.SendEmmt(setup_res.emm.EMMt) != 0) {
        std::cout << "[setup] send emmt to server failed!" << std::endl;
        return -3;
    }
    if (client.SendXset(setup_res.emm.Xset) != 0) {
        std::cout << "[setup] send xset to server failed!" << std::endl;
        return -3;
    }
    return 0;
}

int token(PrFilterClient &client, std::vector<std::string> command, int mm_len, MK mk,
          pr_filter_token_param &token_param, pr_filter_token_res &token_res) {
    std::vector<std::string> words;
    for (int i = 1; i < command.size(); i++) {
        if (command[i] == "-word" || command[i] == "-words" || command[i] == "-w") {
            ++i;
            while (i < command.size()) {
                words.push_back(command[i++]);
            }
        } else {
            std::cout << "[token] wrong command!" << std::endl;
            return -1;
        }
    }
    if(words.size()<2){
        std::cout << "[token] you should enter more than two words!" << std::endl;
        return -1;
    }
    token_param.len = mm_len;
    token_param.mk = std::move(mk);
    token_param.words = words;
    token_res.tokp_vec.clear();
    if (PR_Filter_Token(token_param, token_res) != 0) {
        std::cout << "[token] call PR_Filter_Token failed!" << std::endl;
        return -2;
    }
    tokenChange2File(token_res, "./token.txt");
    return 0;
}

int resolve(PrFilterClient &client, const pr_filter_setup_res& setup_res, pr_filter_token_res token_res,
            std::vector<std::string> words, pr_filter_resolve_param &resolve_param) {
    // call PR_Filter_Search
    std::vector<std::string> c;
    std::vector<std::string> dc;
    std::vector<bool> vaild;
    client.SearchInServer(std::move(token_res), c, dc, vaild);
    if (words.size() < 2) {
        std::cout << "[resolve] you should token first!" << std::endl;
        return -1;
    }

    //call PR_Filter_Resolve
    resolve_param.w1 = words[0];
    resolve_param.wn = words[words.size() - 1];
    resolve_param.mk = setup_res.mk;
    resolve_param.c = c;
    resolve_param.dc = dc;
    resolve_param.DX = setup_res.DX;
    std::vector<std::string> resolve_res;
    if (PR_Filter_Resolve(resolve_param, resolve_res) != 0) {
        std::cout << "[resolve] call PR_Filter_Resolve failed!" << std::endl;
        return -2;
    }

    StrVecRemoveTag(resolve_res);
    std::cout << "resolve result: ";
    for (const auto &ans: resolve_res) {
        cout << ans << " ";
    }
    std::cout << std::endl;
    return 0;
}

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

