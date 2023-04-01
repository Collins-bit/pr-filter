#ifndef PRFILTER_CLIENT_H
#define PRFILTER_CLIENT_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "PrFilter.grpc.pb.h"

#include <pr_filter/pr_filter_client.h>
#include <conj_filter/conj_filter.h>
#include <util/file_operate.h>
#include <util/time_util.h>
#include <util/storage_overhead.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using PrFilter::PrFilterService;
using PrFilter::SetupEmmtRequest;
using PrFilter::SetupXsetRequest;
using PrFilter::SearchRequest;
using PrFilter::SearchResponse;

class PrFilterClient {
public:
    PrFilterClient(std::shared_ptr<Channel> channel) : stub_(PrFilterService::NewStub(channel)) {}

    int SendEmmt(const std::map<std::string, cdc> &EMMt);

    int SendXset(const std::multiset<std::string> &Xset);

    int SearchInServer(pr_filter_token_res token_res, std::vector<std::string> &c, std::vector<std::string> &dc, std::vector<bool> &vaild);

private:
    std::unique_ptr<PrFilterService::Stub> stub_;
};

//pr-filter process
int setup(PrFilterClient &client, std::vector<std::string> command, int &mm_len,
          pr_filter_setup_param &setup_param, pr_filter_setup_res &setup_res);

int token(PrFilterClient &client, std::vector<std::string> command, int mm_len, MK mk,
          pr_filter_token_param &token_param, pr_filter_token_res &token_res);

int resolve(PrFilterClient &client, const pr_filter_setup_res& setup_res, pr_filter_token_res token_res,
            std::vector<std::string> words, pr_filter_resolve_param &resolve_param);

//conj-filter process
int conj_filter_process(std::vector<std::string> command);

#endif //PRFILTER_CLIENT_H
