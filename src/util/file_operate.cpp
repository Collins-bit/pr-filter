#include <util/file_operate.h>

void Get_MM_from_fpath(string path, map<string, vector<string>> &MM, int &max_mm_len) {
    ifstream MM_myfile(path);
    string w, line;
    max_mm_len = 0;
    while (getline(MM_myfile, line)) {
        stringstream input(line);
        string out;
        vector<string> ids;
        input >> out;
        w = out;
        while (input >> out) {
            max_mm_len = max_mm_len > out.size() ? max_mm_len : out.size();
            ids.push_back(out);
        }
        MM[w] = ids;
    }
    cout << "read MM from file success!" << endl;
}

string StrPaddingWithTag(string s, int len) {
    string r = s;
    if (s.size() < len) {
        int count = len - s.size();
        string pad(count, '#');
        r += pad;
    }
    return r;
}

string StrRemoveTag(string s) {
    int pos = s.find_first_of('#');
    if (pos == -1) {
        return s;
    }
    return s.erase(pos, s.size() - pos);
}

void StrVecRemoveTag(vector<string> &strs) {
    for (int i = 0; i < strs.size(); i++) {
        strs[i] = StrRemoveTag(strs[i]);
    }
}

void StrMapRemoveTag(vector<string> strs) {
    for (int i = 0; i < strs.size(); i++) {
        strs[i] = StrRemoveTag(strs[i]);
    }
}

int Deal_mm_same_length(int max_mm_len, map<string, vector<string>> &MM) {
    bool flag = true;
    for (auto mm: MM) {
        vector<string> tmp(mm.second.size());
        for (int i = 0; i < mm.second.size(); i++) {
            if (mm.second[i].size() > max_mm_len) {
                cout << "exists greater than the input value" << endl;
                return -1;
            } else if (mm.second[i].size() < max_mm_len) {
                flag = false;
            }
            tmp[i] = StrPaddingWithTag(mm.second[i], max_mm_len);
        }
        MM[mm.first] = tmp;
    }
    if (flag) {
        cout << "no set MM length." << endl;
    } else {
        cout << "set MM length to be same success!" << endl;
    }
    return 0;
}