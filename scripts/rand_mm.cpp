#include <iostream>
#include <bits/stdc++.h>

using namespace std;

std::multiset<int> num_set;

int rand_num() {
    int num = rand() % 9999999;
    if (num < 1000000) {
        num += 1000000;
    }
    while (num_set.find(num) != num_set.end()) {
        num = rand() % 9999999;
        if (num < 1000000) {
            num += 1000000;
        }
    }
    num_set.insert(num);
    return num;
}

void rand_mm(const std::string &ofpath) {
    std::ofstream os(ofpath, std::ios::app);
    if (!os) {
        cout << "ofstream file failed!" << endl;
        exit(1);
    }
    vector<string> words{"name", "age", "sex"};
    vector<int> w_time{5000, 2000, 2000};
    int time = 0;
    for (int i = 0; i < words.size(); i++) {
        for (int j = i + 1; j < words.size(); j++) {
            os << words[i] << "∩" << words[j] << " ";
            int t = w_time[time++];
            while (t--) {
                os << rand_num() << " ";
            }
            os << "\n";
        }
    }
    os.close();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    num_set.clear();
    rand_mm("../testData/test.txt");
    return 0;
}