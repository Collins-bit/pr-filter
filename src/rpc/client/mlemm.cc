#include <rpc/client.h>

int mlemm_process(std::vector<std::string> command) {
    // get from command
    std::string fpath = "../testData/test_MM_w3.txt";
    int keylen = 32;
    std::vector<std::string> words;
    for (int i = 1; i < command.size(); i++) {
        if ((command[i] == "-fpath" || command[i] == "-f") && i + 1 < command.size()) {
            fpath = command[++i];
        } else if ((command[i] == "-keylen" || command[i] == "-k") && i + 1 < command.size()) {
            keylen = stoi(command[++i]);
        } else if (command[i] == "-word" || command[i] == "-words" || command[i] == "-w") {
            ++i;
            while (i < command.size()) {
                if (command[i] == "-fpath" || command[i] == "-f" || command[i] == "-keylen" || command[i] == "-k") {
                    --i;
                    break;
                }
                words.push_back(command[i++]);
            }
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

    // call MLEMM_Setup
    mlemm_setup_param mlemm_setup_param;
    mlemm_setup_res mlemm_setup_res;
    mlemm_setup_param.MM = MM;
    mlemm_setup_param.lambda = keylen;
#ifdef TEST
    TimeUtil time_util1;
    time_util1.initTime();
    for (int i = 0; i < 100; i++) {
        if (MLEMM_Setup(mlemm_setup_param, mlemm_setup_res) != 0) {
            std::cout << "[setup] call MLEMM_Setup failed!" << std::endl;
            return -2;
        }
    }
    time_util1.endTime("mlemm setup process", 100);
    emmtChange2File(mlemm_setup_res.EMMp, "./emmt.txt");
    xsetChange2File(mlemm_setup_res.X, "./xset.txt");
#else
    if (MLEMM_Setup(mlemm_setup_param, mlemm_setup_res) != 0) {
        std::cout << "[setup] call MLEMM_Setup failed!" << std::endl;
        return -2;
    }
#endif
    std::cout << "setup success!\n" << std::endl;

    // call MLEMM_Token
    mlemm_token_param mlemm_token_param;
    mlemm_token_res mlemm_token_res;
    mlemm_token_param.kp = mlemm_setup_res.kp;
    mlemm_token_param.kx = mlemm_setup_res.kx;
    mlemm_token_param.kenc = mlemm_setup_res.kenc;
    mlemm_token_param.mskp = mlemm_setup_res.mskp;
    mlemm_token_param.words = words;
    if (MLEMM_Token(mlemm_token_param, mlemm_token_res) != 0) {
        std::cout << "[token] call MLEMM_Token failed!" << std::endl;
        return -2;
    }
#ifdef TEST
    mlemmTokenChange2File(mlemm_token_res, "./mlemm_token.txt");
#endif
    std::cout << "token success!\n" << std::endl;

    // call MLEMM_Search
    mlemm_search_param mlemm_search_param;
    mlemm_search_param.tokp = mlemm_token_res.tokp;
    mlemm_search_param.k_l123_enc = mlemm_token_res.k_l123_enc;
    mlemm_search_param.kx = mlemm_token_res.kx;
    mlemm_search_param.EMMp = mlemm_setup_res.EMMp;
    mlemm_search_param.X = mlemm_setup_res.X;
    std::vector<std::string> mlemm_ev;
#ifdef TEST
    TimeUtil time_util2;
    time_util2.initTime();
    for (int i = 0; i < 10000; i++) {
        if (MLEMM_Search(mlemm_search_param, mlemm_ev) != 0) {
            std::cout << "call MLEMM_Search failed!" << std::endl;
            return -2;
        }
    }
    time_util2.endTime("search process", 10000);
#else
    if (MLEMM_Search(mlemm_search_param, mlemm_ev) != 0) {
        std::cout << "call MLEMM_Search failed!" << std::endl;
        return -2;
    }
#endif
    std::cout << "search success!\n" << std::endl;

    // call MLEMM_Resolve
    mlemm_resolve_param mlemm_resolve_param;
    mlemm_resolve_param.kenc = mlemm_setup_res.kenc;
    mlemm_resolve_param.ev = mlemm_ev;
    std::vector<std::string> mlemm_ans;
    if (MLEMM_Resolve(mlemm_resolve_param, mlemm_ans) != 0) {
        std::cout << "call MLEMM_Resolve failed!" << std::endl;
        return -2;
    }
    StrVecRemoveTag(mlemm_ans);
    std::cout << "resolve result: ";
    for (const auto &ans: mlemm_ans) {
        cout << ans << " ";
    }
    std::cout << std::endl;
    std::cout << "resolve success!" << std::endl;

    return 0;
}
