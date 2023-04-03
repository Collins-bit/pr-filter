#include <iostream>
#include <bits/stdc++.h>

using namespace std;

std::multiset<int> num_set;

vector<vector<int>> w_time{
        {15,   10,   10,   10},
        {15,   30,   30,   30},
        {15,   50,   50,   50},
        {15,   80,   80,   80},
        {15,   100,  100,  100},
        {15,   300,  300,  300},
        {15,   500,  500,  500},
        {15,   800,  800,  800},
        {15,   1000, 1000, 1000},
        {15,   3000, 3000, 3000},
        {15,   5000, 5000, 5000},

        {2000, 10,   10,   10},
        {2000, 30,   30,   30},
        {2000, 50,   50,   50},
        {2000, 80,   80,   80},
        {2000, 100,  100,  100},
        {2000, 300,  300,  300},
        {2000, 500,  500,  500},
        {2000, 800,  800,  800},
        {2000, 1000, 1000, 1000},
        {2000, 3000, 3000, 3000},
        {2000, 5000, 5000, 5000},

        {10,   15,   15,   15},
        {30,   15,   15,   15},
        {50,   15,   15,   15},
        {80,   15,   15,   15},
        {100,  15,   15,   15},
        {300,  15,   15,   15},
        {500,  15,   15,   15},
        {800,  15,   15,   15},
        {1000, 15,   15,   15},
        {3000, 15,   15,   15},
        {5000, 15,   15,   15},

        {10,   2000, 2000, 2000},
        {30,   2000, 2000, 2000},
        {50,   2000, 2000, 2000},
        {80,   2000, 2000, 2000},
        {100,  2000, 2000, 2000},
        {300,  2000, 2000, 2000},
        {500,  2000, 2000, 2000},
        {800,  2000, 2000, 2000},
        {1000, 2000, 2000, 2000},
        {3000, 2000, 2000, 2000},
        {5000, 2000, 2000, 2000},
};


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

void rand_mm(const std::string &ofpath, vector<int> input_w_time) {
    std::ofstream os(ofpath, std::ios::app);
    if (!os) {
        cout << "ofstream file failed!" << endl;
        exit(1);
    }
    vector<string> words{"name", "age", "sex", "grade"};
    int time = 0;
    for (int i = 0; i < words.size(); i++) {
        for (int j = i + 1; j < words.size(); j++) {
            for (int k = j + 1; k < words.size(); k++) {
                os << words[i] << "∩" << words[j] << "∩" << words[k] << " ";
                int t = input_w_time[time++];
                while (t--) {
                    os << rand_num() << " ";
                }
                os << "\n";
            }
        }
    }
    os.close();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    num_set.clear();
    vector<int> input_w_time = w_time[31];
    cout<<"current w_time: ";
    for(auto w : input_w_time){
        cout<<w << " ";
    }
    cout<<endl;
    rand_mm("../testData/test.txt", input_w_time);
    cout<<"generate random mm success!"<<endl;
    return 0;
}