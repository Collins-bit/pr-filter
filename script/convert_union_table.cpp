#include <iostream>
#include <bits/stdc++.h>

using namespace std;

void get_MM_from_inverted_file(int num, const std::string &path, std::map<std::string, std::vector<std::string>> &MM) {
    std::ifstream MM_myfile(path);
    std::string w, line;
    while (getline(MM_myfile, line)) {
        if (num <= 0) {
            break;
        }
        std::stringstream input(line);
        std::string out;
        std::vector<std::string> ids;
        input >> out;
        w = out;
        while (input >> out) {
            ids.push_back(out);
            --num;
        }
        MM[w] = ids;
    }
}

void convert_union_file(std::map<std::string, std::vector<std::string>> inverted_MM, const std::string &ofpath) {
    int sum = 0;
    std::ofstream os(ofpath, std::ios::app);
    auto i = inverted_MM.begin();
    for (; i != inverted_MM.end(); i++) {
        std::unordered_set<std::string> k1(i->second.begin(), i->second.end());
        auto j = i;
        ++j;
        for (; j != inverted_MM.end(); j++) {
            os << i->first << "∩" << j->first;
            bool first=true;
            for (const auto &z: j->second) {
                if (k1.find(z) != k1.end()) {
                    if(first){
                        os << " ";
                        first=false;
                    }
                    os << z << " ";
                }
            }
            os << "\n";
        }
        sum++;
        if (sum == 2) break;
    }
    os.close();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
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

    cout << "convert success!" << endl;

    return 0;
}