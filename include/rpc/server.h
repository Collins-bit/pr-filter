#ifndef PRFILTER_SERVER_H
#define PRFILTER_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "PrFilter.grpc.pb.h"

#include <algorithm/pr_filter_server.h>
#include <util/time_util.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;
using PrFilter::PrFilterService;
using PrFilter::SetupEmmtRequest;
using PrFilter::SetupXsetRequest;
using PrFilter::SearchRequest;
using PrFilter::SearchResponse;

extern std::map<std::string, cdc> EMMt;
extern std::multiset<std::string> Xset;

class PrFilterServiceImpl final : public PrFilterService::Service {
    Status SetupEmmt(ServerContext *context, ServerReader<SetupEmmtRequest> *reader, google::protobuf::Empty*) override;

    Status SetupXset(ServerContext *context, ServerReader<SetupXsetRequest> *reader, google::protobuf::Empty*) override;

    Status Search(ServerContext *context, const SearchRequest *request, SearchResponse *searchResponse) override;
};

#endif //PRFILTER_SERVER_H
