#ifndef PRFILTER_CLIENT_H
#define PRFILTER_CLIENT_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "PrFilter.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using PrFilter::PrFilterService;
using PrFilter::SetupEmmtRequest;
using PrFilter::SetupEmmtRespone;
using PrFilter::SetupXsetRequest;
using PrFilter::SetupXsetRespone;
using PrFilter::SearchRequest;
using PrFilter::SearchRespone;

class PrFilterClient {
public:
    PrFilterClient(std::shared_ptr<Channel> channel) : stub_(PrFilterService::NewStub(channel)) {}

    std::string SayHello();

private:
    std::unique_ptr<PrFilterService::Stub> stub_;
};

#endif //PRFILTER_CLIENT_H
