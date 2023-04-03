#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int find_w123_from_w(const string &word, string &w1, string &w2, string &w3) {
    size_t pos1 = word.find("∩");
    if (pos1 == string::npos) {
        // 没有找到第一个分割符
        return -1;
    }
    w1 = word.substr(0, pos1);
    size_t pos2 = word.find("∩", pos1 + 3);
    if (pos2 == string::npos) {
        // 没有找到第二个分割符
        return -1;
    }
    w2 = word.substr(pos1 + 3, pos2 - pos1 - 3);
    w3 = word.substr(pos2 + 3);
    return 0;
}

int get_MM_from_inverted_file(const std::string &path, std::map<std::string, std::vector<std::string>> &MM) {
    std::ifstream MM_myfile(path);
    std::string w, line;
    while (getline(MM_myfile, line)) {
        std::stringstream input(line);
        std::string out;
        std::vector<std::string> ids;
        input >> out;
        w = out;
        while (input >> out) {
            ids.push_back(out);
        }
        MM[w] = ids;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "args not enough!" << endl;
        return 0;
    }
    std::string num_str = &argv[1][0];
    int num = std::stoi(num_str);
    std::string input_fpath = &argv[2][0];
    std::map<std::string, std::vector<std::string>> MM;
    get_MM_from_inverted_file(input_fpath, MM);

    multiset<string> set;
    for (auto mm: MM) {
        if (num == 0) break;
        string w1, w2, w3;
        find_w123_from_w(mm.first, w1, w2, w3);
        if (set.find(w1)==set.end()) {
            cout << w1 << " ";
            set.insert(w1);
            --num;
        }
        if (set.find(w2)==set.end()) {
            cout << w2 << " ";
            set.insert(w2);
            --num;
        }
        if (set.find(w3)==set.end()) {
            cout << w3 << " ";
            set.insert(w3);
            --num;
        }
    }
    return 0;
}