#ifndef TKFILTER_CLIENT_H
#define TKFILTER_CLIENT_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "TKFilter.grpc.pb.h"

#include <tk_filter/tk_filter_client.h>
#include <conj_filter/conj_filter.h>
#include <mlemm/mlemm.h>
#include <util/file_operate.h>
#include <util/time_util.h>
#include <util/storage_overhead.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using TKFilter::TKFilterService;
using TKFilter::SetupEmmtRequest;
using TKFilter::SetupXsetRequest;
using TKFilter::SearchRequest;
using TKFilter::SearchResponse;

class TKFilterClient {
public:
    TKFilterClient(std::shared_ptr<Channel> channel) : stub_(TKFilterService::NewStub(channel)) {}

    int SendEmmt(const std::map<std::string, cdc> &EMMt);

    int SendXset(const std::multiset<std::string> &Xset);

    int SearchInServer(tk_filter_token_res token_res, std::vector<std::string> &c, std::vector<std::string> &dc, std::vector<bool> &vaild);

private:
    std::unique_ptr<TKFilterService::Stub> stub_;
};

//tk-filter process
int setup(TKFilterClient &client, std::vector<std::string> command, int &mm_len,
          tk_filter_setup_param &setup_param, tk_filter_setup_res &setup_res);

int token(TKFilterClient &client, std::vector<std::string> command, int mm_len, MK mk,
          tk_filter_token_param &token_param, tk_filter_token_res &token_res);

int resolve(TKFilterClient &client, const tk_filter_setup_res& setup_res, tk_filter_token_res token_res,
            std::vector<std::string> words, tk_filter_resolve_param &resolve_param);

//conj-filter process
int conj_filter_process(std::vector<std::string> command);

//mlemm process
int mlemm_process(std::vector<std::string> command);

#endif //TKFILTER_CLIENT_H
