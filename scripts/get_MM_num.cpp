#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int get_MM_from_inverted_file(const std::string &path, std::map<std::string, std::vector<std::string>> &MM) {
    int num = 0;
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
            ++num;
        }
        MM[w] = ids;
    }
    return num;
}

void convert_union_file(int num, std::map<std::string,
        std::vector<std::string>> MM, const std::string &ofpath) {
    std::ofstream os(ofpath, std::ios::app);
    for (auto mm: MM) {
        if (num <= 0) break;
        os << mm.first << " ";
        for (auto z: mm.second) {
            os << z << " ";
            --num;
        }
        os << "\n";
    }
    os.close();
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        cout << "args not enough!" << endl;
        return 0;
    }
    std::string input_fpath = &argv[1][0];
    std::map<std::string, std::vector<std::string>> MM;
    int sum = get_MM_from_inverted_file(input_fpath, MM);
    cout << "original file have " << sum << " pair mm" << endl;

    if (argc == 4) {
        std::string mm_num_str = &argv[2][0];
        int mm_num = std::stoi(mm_num_str);
        if(mm_num>sum){
            cout << "input num too large! failed!" << endl;
            return 0;
        }
        std::string output_fpath = &argv[3][0];
        convert_union_file(mm_num, MM, output_fpath);
        cout << "convert success! (num: " << mm_num << ")" << endl;
    }

    return 0;
}