#include <rpc/client.h>

void handleInputParam(PrFilterClient &client) {
    int mm_len;
    pr_filter_setup_param setup_param;
    pr_filter_setup_res setup_res;
    pr_filter_token_param token_param;
    pr_filter_token_res token_res;
    pr_filter_resolve_param resolve_param;
    while (true) {
        std::cout << "please input command (support setup, token, resolve, stop): \n";
        std::string command;
        getline(std::cin, command);
        // split command
        std::istringstream in(command);
        std::vector<std::string> o_vec;
        std::string str;
        while (getline(in, str, ' ')) {
            o_vec.push_back(str);
        }
        // input command and implement
        if (o_vec.empty()) continue;
        if (o_vec[0] == "stop") break;
        else if (o_vec[0] == "setup") {
            if (setup(client, o_vec, mm_len, setup_param, setup_res) == 0) {
                std::cout << "setup success!" << std::endl;
            }
        } else if (o_vec[0] == "token") {
            if (token(client, o_vec, mm_len, setup_res.mk, token_param, token_res) == 0) {
                std::cout << "token success!" << std::endl;
            }
        } else if (o_vec[0] == "resolve") {
            if (resolve(client, o_vec, setup_res, token_res, token_param.words, resolve_param) == 0) {
                std::cout << "resolve success!" << std::endl;
            }
        } else
            std::cout << "wrong command!" << std::endl;
        std::cout<<std::endl;
    }
}

int main(int argc, char **argv) {
    PrFilterClient client(grpc::CreateChannel("127.0.0.1:12520", grpc::InsecureChannelCredentials()));
    handleInputParam(client);
    return 0;
}