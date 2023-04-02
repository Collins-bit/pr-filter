#include <util/storage_overhead.h>

size_t getFileSize(const char *fileName) {
    if (fileName == NULL) {
        return 0;
    }
    struct stat statbuf;
    stat(fileName, &statbuf);
    size_t filesize = statbuf.st_size;
    return filesize;
}

int emmtChange2File(const map<string, cdc> &EMMt, const string &fpath) {
    // output file
    ofstream os(fpath, ios::binary | ios::out);
    if (!os) {
        cout << "ofstream file failed: " << fpath << endl;
        return -1;
    }
    for (const auto &emmt: EMMt) {
        os << emmt.first;
        for (const auto &c: emmt.second.c) {
            os << c;
        }
        for (const auto &dc: emmt.second.dc) {
            os << dc;
        }
        os << "\n";
    }
    os.close();
    // compute size
    size_t file_size = getFileSize(fpath.c_str());
    cout << "size of emmt is: " << file_size << " bytes" << endl;
    // delete file
    if (remove(fpath.c_str()) != 0) {
        cout << "[error] delete file failed " << endl;
    }
    return 0;
}

int xsetChange2File(const multiset<string> &Xset, const string &fpath) {
    // output file
    ofstream os(fpath, ios::binary | ios::out);
    if (!os) {
        cout << "ofstream file failed: " << fpath << endl;
        return -1;
    }
    int time = 0;
    for (const auto &xset: Xset) {
        if (time == 50) {
            os << "\n";
            time = 0;
        }
        os << xset;
        ++time;
    }
    os.close();
    // compute size
    size_t file_size = getFileSize(fpath.c_str());
    cout << "size of xset is: " << file_size << " bytes" << endl;
    // delete file
    if (remove(fpath.c_str()) != 0) {
        cout << "[error] delete file failed " << endl;
    }
    return 0;
}

int tokenChange2File(const pr_filter_token_res &token, const string &fpath) {
    // output file
    ofstream os(fpath, ios::binary | ios::out);
    if (!os) {
        cout << "ofstream file failed: " << fpath << endl;
        return -1;
    }
    os << token.tokp << token.k_w12_enc << "\n";
    for (const auto &key: token.tokp_vec) {
        os << key.kx;
        for (auto ck: key.CK) {
            os << ck;
        }
        for (const auto &p2: key.P2) {
            for (auto pp2: p2) {
                os << pp2;
            }
        }
        for (const auto &keyphi: key.KeyPhi) {
            os << keyphi;
        }
        os << "\n";
    }
    os.close();
    // compute size
    size_t file_size = getFileSize(fpath.c_str());
    cout << "size of token is: " << file_size << " bytes" << endl;
    // delete file
    if (remove(fpath.c_str()) != 0) {
        cout << "[error] delete file failed " << endl;
    }
    return 0;
}


int conjTokenChange2File(const conj_filter_token_res &token, const string &fpath) {
    // output file
    ofstream os(fpath, ios::binary | ios::out);
    if (!os) {
        cout << "ofstream file failed: " << fpath << endl;
        return -1;
    }
    os << token.tokp << token.k_l12_enc << "\n";
    for (const auto &key: token.kx) {
        os << key << " ";
    }
    os.close();
    // compute size
    size_t file_size = getFileSize(fpath.c_str());
    cout << "size of token is: " << file_size << " bytes" << endl;
    // delete file
    if (remove(fpath.c_str()) != 0) {
        cout << "[error] delete file failed " << endl;
    }
    return 0;
}

int mlemmTokenChange2File(const mlemm_token_res &token, const string &fpath) {
    // output file
    ofstream os(fpath, ios::binary | ios::out);
    if (!os) {
        cout << "ofstream file failed: " << fpath << endl;
        return -1;
    }
    os << token.tokp << token.k_l123_enc << "\n";
    for (const auto &key: token.kx) {
        os << key << " ";
    }
    os.close();
    // compute size
    size_t file_size = getFileSize(fpath.c_str());
    cout << "size of token is: " << file_size << " bytes" << endl;
    // delete file
    if (remove(fpath.c_str()) != 0) {
        cout << "[error] delete file failed " << endl;
    }
    return 0;
}
