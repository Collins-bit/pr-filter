#include <rpc/client.h>

std::string PrFilterClient::SayHello() {
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
    if (status.ok()) {
        return "success";
    } else {
        std::cout << status.error_code() << ": " << status.error_message() << std::endl;
        return "failed";
    }
}


