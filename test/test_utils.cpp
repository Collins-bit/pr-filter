#include "test_utils.h"

bool compare(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

vector<string> get_words_from_mm(std::map<std::string, std::vector<std::string>> MM) {
    std::map<std::string, int> frequency;
    vector<string> words;
    for (auto mm: MM) {
        string w1, w2;
        find_w12_from_w(mm.first, w1, w2);
        frequency[w1]+=2;
        frequency[w2]++;
    }
    std::vector<std::pair<std::string, int>> sortedArr(frequency.begin(), frequency.end());
    std::sort(sortedArr.begin(), sortedArr.end(), compare);
    for (const auto& pair : sortedArr) {
        words.push_back(pair.first);
    }
    return words;
}