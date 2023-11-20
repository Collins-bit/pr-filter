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

void rand_mm(const std::string &ofpath, long word_num, int match_time) {
    std::ofstream os(ofpath, std::ios::app);
    if (!os) {
        cout << "ofstream file failed!" << endl;
        exit(1);
    }
    os << "name∩age ";
    for (int i = 1000000; i <= 1000000+word_num; i++) {
        os << i << " ";
    }
    os << "\n";
    os << "name∩sex ";
    for (int i = 1000000; i <= 1000000+match_time; i++) {
        os << i << " ";
    }
    os << "\n";
    os << "age∩sex ";
    for (int i = 1000000; i <= 1000000+match_time; i++) {
        os << i << " ";
    }
    os << "\n";
    os.close();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    num_set.clear();
    rand_mm("../testData/test.txt", 1000000, 500);
    return 0;
}