#ifndef TKFILTER_SERVER_H
#define TKFILTER_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "TKFilter.grpc.pb.h"

#include <tk_filter/tk_filter_server.h>
#include <util/time_util.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::Status;
using TKFilter::TKFilterService;
using TKFilter::SetupEmmtRequest;
using TKFilter::SetupXsetRequest;
using TKFilter::SearchRequest;
using TKFilter::SearchResponse;

extern std::map<string, std::vector<c_ectr>> EMMt;
extern std::multiset<std::string> Xset;

class TKFilterServiceImpl final : public TKFilterService::Service {
    Status SetupEmmt(ServerContext *context, ServerReader<SetupEmmtRequest> *reader, google::protobuf::Empty*) override;

    Status SetupXset(ServerContext *context, ServerReader<SetupXsetRequest> *reader, google::protobuf::Empty*) override;

    Status Search(ServerContext *context, const SearchRequest *request, SearchResponse *searchResponse) override;
};

#endif //TKFILTER_SERVER_H
