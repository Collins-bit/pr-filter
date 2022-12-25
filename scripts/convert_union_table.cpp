#include <iostream>
#include <bits/stdc++.h>

using namespace std;

void get_MM_from_inverted_file(const std::string &path, std::map<std::string, std::vector<std::string>> &MM) {
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
}

void convert_union_file(int num, std::map<std::string,
        std::vector<std::string>> inverted_MM, const std::string &ofpath) {
    std::ofstream os(ofpath, std::ios::app);
    if (!os) {
        cout << "ofstream file failed!" << endl;
        exit(1);
    }
    auto i = inverted_MM.begin();
    for (; i != inverted_MM.end(); i++) {
        if (num <= 0) break;
        std::unordered_set<std::string> k1(i->second.begin(), i->second.end());
        auto j = i;
        ++j;
        for (; j != inverted_MM.end(); j++) {
            if (num <= 0) break;
            bool flag = false;
            for (const auto &z: j->second) {
                if (k1.find(z) != k1.end()) {
                    if (!flag) {
                        os << i->first << "∩" << j->first << " ";
                        flag = true;
                    }
                    os << z << " ";
                    --num;
                }
            }
            if (flag) os << "\n";
        }
    }
    os.close();
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

    get_MM_from_inverted_file(input_fpath, inverted_MM);

    convert_union_file(mm_num, inverted_MM, output_fpath);

    cout << "convert success! (num: " << mm_num << ")" << endl;

    return 0;
}