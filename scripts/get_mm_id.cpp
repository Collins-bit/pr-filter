#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int find_w12_from_w(std::string word, std::string &w1, std::string &w2) {
    if (word == "") {
        w1 = "";
        w2 = "";
        return 0;
    }
    int idx = word.find("∩");
    if (idx == -1) {
        std::cout << "substr without ∩, wrong msg: " << word << std::endl;
        return -1;
    }
    w1 = word.substr(0, idx);
    w2 = word.substr(idx + 3, word.size() - idx);
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

    multiset<string> check_one;

    string last = "";
    for (auto mm: MM) {
        if (num == 0) break;
        string w1, w2;
        find_w12_from_w(mm.first, w1, w2);
        if (check_one.find(w1) == check_one.end()) {
            check_one.insert(w1);
            cout << w1 << " ";
            --num;
        }
        if (check_one.find(w2) == check_one.end()) {
            check_one.insert(w2);
            cout << w2 << " ";
            --num;
        }
    }
    return 0;
}