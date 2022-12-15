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

class PrFilterClient
{
public:
    PrFilterClient(std::shared_ptr<Channel> channel) : stub_(PrFilterService::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello()
    {
        // Data we are sending to the server.
        SearchRequest request;

        // Container for the data we expect from the server.
        SearchRespone response;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->Search(&context, request, &response);

        // Act upon its status.
        if (status.ok()){
            return "success";
        } else {
            std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            return "failed";
        }
    }

private:
    std::unique_ptr<PrFilterService::Stub> stub_;
};

int main(int argc, char **argv)
{
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    PrFilterClient greeter(grpc::CreateChannel("127.0.0.1:8888", grpc::InsecureChannelCredentials()));
    std::string reply = greeter.SayHello();
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}