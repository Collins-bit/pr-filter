#include <util/file_operate.h>

void Get_MM_from_fpath(std::string path, std::map<std::string, std::vector<std::string>> &MM, int &max_mm_len)
{
    std::ifstream MM_myfile(path);
    std::string w, line;
    max_mm_len = 0;
    while (getline(MM_myfile, line))
    {
        std::stringstream input(line);
        std::string out;
        std::vector<std::string> ids;
        input >> out;
        w = out;
        while (input >> out)
        {
            max_mm_len = max_mm_len > out.size() ? max_mm_len : out.size();
            ids.push_back(out);
        }
        MM[w] = ids;
    }
}

std::string StrPaddingWithTag(std::string s, int len)
{
    std::string r = s;
    if (s.length() < len)
    {
        int count = len - s.size();
        std::string pad(count, '#');
        r += pad;
    }
    return r;
}

std::string StrRemoveTag(std::string s)
{
    int pos = s.find_first_of('#');
    if (pos == -1)
    {
        return s;
    }
    return s.erase(pos, s.size() - pos);
}

void StrVecRemoveTag(std::vector<std::string> &strs)
{
    for (int i = 0; i < strs.size(); i++)
    {
        strs[i] = StrRemoveTag(strs[i]);
    }
}

void StrMapRemoveTag(std::vector<std::string> strs)
{
    for (int i = 0; i < strs.size(); i++)
    {
        strs[i] = StrRemoveTag(strs[i]);
    }
}

int Deal_mm_same_length(int max_mm_len, std::map<std::string, std::vector<std::string>> &MM)
{
    bool flag = true;
    for (auto mm : MM)
    {
        std::vector<std::string> tmp(mm.second.size());
        for (int i = 0; i < mm.second.size(); i++)
        {
            if (mm.second[i].size() > max_mm_len)
            {
                std::cout << "exists greater than the input value" << std::endl;
                return -1;
            }
            else if (mm.second[i].size() < max_mm_len)
            {
                flag = false;
            }
            tmp[i] = StrPaddingWithTag(mm.second[i], max_mm_len);
        }
        MM[mm.first] = tmp;
    }
    if (flag)
    {
        std::cout << "not processed" << std::endl;
    }
    else
    {
        std::cout << "processed success" << std::endl;
    }
    return 0;
}