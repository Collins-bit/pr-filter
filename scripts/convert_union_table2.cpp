/**
 * Conj-filter、TK-filter提取关键字
 */
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int find_w12_from_w(string word, string &w1, string &w2) {
    if (word == "") {
        w1 = "";
        w2 = "";
        return 0;
    }
    int idx = word.find("∩");
    if (idx == -1) {
        cout << "substr without ∩, wrong msg: " << word << endl;
        return -1;
    }
    w1 = word.substr(0, idx);
    w2 = word.substr(idx + 3, word.size() - idx);
    return 0;
}

void get_MM_from_inverted_file(int num, const std::string &path, std::map<std::string, std::vector<std::string>> &MM) {
    int n = num, word_num=0;
    std::ifstream MM_myfile(path);
    std::string w, line;
    while (getline(MM_myfile, line)) {
        if (n <= 0) break;
        std::stringstream input(line);
        std::string out;
        std::vector<std::string> ids;
        input >> out;
        w = out;
        while (input >> out) {
            n--;
            ids.push_back(out);
        }
        MM[w] = ids;
        word_num++;
    }
    cout << "convert num: " << num - n << endl;
    cout << "words num: " << word_num << endl;
}

void convert_union_file(std::map<std::string, std::vector<std::string>> inverted_MM, const std::string &ofpath) {
    int sum = 0;
    std::ofstream os(ofpath, std::ios::app);
    if (!os) {
        cout << "ofstream file failed!" << endl;
        exit(1);
    }
    auto i = inverted_MM.begin();
    for (; i != inverted_MM.end(); i++) {
        std::unordered_set<std::string> k1(i->second.begin(), i->second.end());
        auto j = i;
        ++j;
        for (; j != inverted_MM.end(); j++) {
            bool flag = false;
            for (const auto &z: j->second) {
                if (k1.find(z) != k1.end()) {
                    if (!flag) {
                        os << i->first << "∩" << j->first << " ";
                        flag = true;
                    }
                    os << z << " ";
                    sum++;
                }
            }
            if (flag) os << "\n";
        }
    }
    os.close();
    cout << "sum: " << sum << endl;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cout << "args not enough, must over three!" << endl;
        return 0;
    }
    std::string mm_num_str = &argv[1][0];
    int mm_num = std::stoi(mm_num_str);
    std::string input_fpath = &argv[2][0];
    std::string output_fpath = &argv[3][0];

    std::map<std::string, std::vector<std::string>> inverted_MM;

    get_MM_from_inverted_file(mm_num, input_fpath, inverted_MM);

    convert_union_file(inverted_MM, output_fpath);

    cout << "convert success! (num: " << mm_num << ")" << endl;

    return 0;
}