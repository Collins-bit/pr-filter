#ifndef PRFILTER_SERVER_H
#define PRFILTER_SERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "PrFilter.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using PrFilter::PrFilterService;
using PrFilter::SetupEmmtRequest;
using PrFilter::SetupEmmtRespone;
using PrFilter::SetupXsetRequest;
using PrFilter::SetupXsetRespone;
using PrFilter::SearchRequest;
using PrFilter::SearchRespone;

class PrFilterServiceImpl final : public PrFilterService::Service {
    Status SetupEmmt(ServerContext *context, const SetupEmmtRequest *request, SetupEmmtRespone *respone) override;

    Status SetupXset(ServerContext *context, const SetupXsetRequest *request, SetupXsetRespone *respone) override;

    Status Search(ServerContext *context, const SearchRequest *request, SearchRespone *respone) override;
};

#endif //PRFILTER_SERVER_H
