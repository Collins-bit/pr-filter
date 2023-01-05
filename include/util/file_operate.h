#ifndef FILE_OPERATE
#define FILE_OPERATE

#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

string StrPaddingWithTag(string s, int len);

string StrRemoveTag(string s);

void StrVecRemoveTag(vector<string> &strs);

void Get_MM_from_fpath(string path, map<string, vector<string>> &MM, int &max_mm_len);

int Deal_mm_same_length(int max_mm_len, map<string, vector<string>> &MM);

#endif // FILE_OPERATE
