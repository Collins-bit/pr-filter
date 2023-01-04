#ifndef PRFILTER_STORAGE_OVERHEAD_H
#define PRFILTER_STORAGE_OVERHEAD_H

#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <cstdio>
#include <algorithm/pr_filter_client.h>
#include <conj_filter/conj_filter.h>

using namespace std;

int emmtChange2File(const std::map<std::string, cdc> &EMMt, const string &fpath);

int xsetChange2File(const std::multiset<std::string>& Xset, const string &fpath);

int tokenChange2File(const pr_filter_token_res& token, const string &fpath);

int conjTokenChange2File(const conj_filter_token_res &token, const string &fpath);

#endif //PRFILTER_STORAGE_OVERHEAD_H
