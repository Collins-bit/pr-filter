#ifndef FILE_OPERATE
#define FILE_OPERATE

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

std::string StrPaddingWithTag(std::string s, int len);

std::string StrRemoveTag(std::string s);

void StrVecRemoveTag(std::vector<std::string> &strs);

void Get_MM_from_fpath(std::string path, std::map<std::string, std::vector<std::string>> &MM, int &max_mm_len);

int Deal_mm_same_length(int max_mm_len, std::map<std::string, std::vector<std::string>> &MM);

#endif // FILE_OPERATE
