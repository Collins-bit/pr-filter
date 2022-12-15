#include <rpc/client.h>

int main(int argc, char **argv) {
    PrFilterClient client(grpc::CreateChannel("127.0.0.1:12520", grpc::InsecureChannelCredentials()));
    std::string reply = client.SayHello();
    std::cout << "Greeter received: " << reply << std::endl;
    return 0;
}