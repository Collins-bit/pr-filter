#include <rpc/client.h>
#include <utility>

int setup(TKFilterClient &client, std::vector<std::string> command, int &mm_len,
          tk_filter_setup_param &setup_param, tk_filter_setup_res &setup_res) {
    // get from command
    std::string fpath = "../testData/test_MM.txt";
    int keylen = 32;
    for (int i = 1; i < command.size(); i += 2) {
        if ((command[i] == "-fpath" || command[i] == "-f") && i + 1 < command.size()) {
            fpath = command[i + 1];
        } else if ((command[i] == "-keylen" || command[i] == "-k") && i + 1 < command.size()) {
            keylen = stoi(command[i + 1]);
        } else {
            std::cout << "[setup] wrong command!" << std::endl;
            return -1;
        }
    }
    // get mm from txt
    std::map<std::string, std::vector<std::string>> MM;
    int max_mm_len;
    Get_MM_from_fpath(fpath, MM, max_mm_len);
    Deal_mm_same_length(max_mm_len, MM);
    mm_len = max_mm_len;
    // init setup_param
    setup_param.MM = MM;
    setup_param.lambda = keylen;
    // call TK_Filter_Setup
#ifdef TEST
    TimeUtil time_util;
    time_util.initTime();
    for (int i = 0; i < 100; i++) {
        if (TK_Filter_Setup(setup_param, setup_res) != 0) {
            std::cout << "[setup] call TK_Filter_Setup failed!" << std::endl;
            return -2;
        }
    }
    time_util.endTime("setup process", 100);
    emmtChange2File(setup_res.emm.EMMt, "./emmt.txt");
    xsetChange2File(setup_res.emm.Xset, "./xset.txt");
    dxChange2File(setup_res.DX, "./dx.txt");
#else
    if (TK_Filter_Setup(setup_param, setup_res) != 0) {
        std::cout << "[setup] call TK_Filter_Setup failed!" << std::endl;
        return -2;
    }
#endif
    // send to server
    if (client.SendEmmt(setup_res.emm.EMMt) != 0) {
        std::cout << "[setup] send emmt to server failed!" << std::endl;
        return -3;
    }
    if (client.SendXset(setup_res.emm.X) != 0) {
        std::cout << "[setup] send xset to server failed!" << std::endl;
        return -3;
    }
    return 0;
}

int token(TKFilterClient &client, std::vector<std::string> command, int mm_len, MK mk,
          tk_filter_token_param &token_param, tk_filter_token_res &token_res) {
    std::vector<std::string> words;
    for (int i = 1; i < command.size(); i++) {
        if (command[i] == "-word" || command[i] == "-words" || command[i] == "-w") {
            ++i;
            while (i < command.size()) {
                words.push_back(command[i++]);
            }
        } else {
            std::cout << "[token] wrong command!" << std::endl;
            return -1;
        }
    }
    if (words.size() < 2) {
        std::cout << "[token] you should enter more than two words!" << std::endl;
        return -1;
    }
    token_param.len = mm_len;
    token_param.mk = std::move(mk);
    token_param.words = words;
    token_res.ck.clear();
    if (TK_Filter_Token(token_param, token_res) != 0) {
        std::cout << "[token] call TK_Filter_Token failed!" << std::endl;
        return -2;
    }
#ifdef TEST
    tokenChange2File(token_res, "./token.txt");
#endif
    return 0;
}

int resolve(TKFilterClient &client, const tk_filter_setup_res &setup_res, tk_filter_token_res token_res,
            std::vector<std::string> words, tk_filter_resolve_param &resolve_param) {
    // call TK_Filter_Search
    std::vector<c_ectr> ct;
    client.SearchInServer(std::move(token_res), ct);
    if (words.size() < 2) {
        std::cout << "[resolve] you should token first!" << std::endl;
        return -1;
    }

    //call TK_Filter_Resolve
    resolve_param.w1 = words[0];
    resolve_param.wn = words[words.size() - 1];
    resolve_param.mk = setup_res.mk;
    resolve_param.ct = ct;
    std::vector<std::string> resolve_res;
    if (TK_Filter_Resolve(resolve_param, resolve_res) != 0) {
        std::cout << "[resolve] call TK_Filter_Resolve failed!" << std::endl;
        return -2;
    }

    StrVecRemoveTag(resolve_res);
    std::cout << "resolve result: ";
    for (const auto &ans: resolve_res) {
        cout << ans << " ";
    }
    std::cout << std::endl;
    return 0;
}
