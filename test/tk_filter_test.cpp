#include <iostream>
#include <tk_filter/tk_filter_util.h>
#include <tk_filter/tk_filter_client.h>
#include <tk_filter/tk_filter_server.h>
#include <util/file_operate.h>
using namespace std;

int main() {
    map<string, vector<string>> MM = {
            {"name∩age",   {"id1", "id2"}},
            {"name∩sex", {"id2", "id3"}},
            {"name∩race",  {"id2"}},
    };
    vector<string> words = {"name", "age", "sex", "race"};

    // call TK_Filter_Setup
    tk_filter_setup_param tk_filter_setup_param;
    tk_filter_setup_res tk_filter_setup_res;
    tk_filter_setup_param.MM = MM;
    tk_filter_setup_param.lambda = 32;
    if (TK_Filter_Setup(tk_filter_setup_param, tk_filter_setup_res) != 0) {
        cout << "[setup] call TK_Filter_Setup failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Setup success!\n" << endl;

    // call TK_Filter_Token
    tk_filter_token_param tk_filter_token_param;
    tk_filter_token_res tk_filter_token_res;
    tk_filter_token_param.mk = tk_filter_setup_res.mk;
    tk_filter_token_param.words = words;
    tk_filter_token_param.len = 3;
    if (TK_Filter_Token(tk_filter_token_param, tk_filter_token_res) != 0) {
        cout << "[token] call TK_Filter_Token failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Token success!\n" << endl;

    // call TK_Filter_Search
    tk_filter_search_param tk_filter_search_param;
    tk_filter_search_param.tokp = tk_filter_token_res.tokp;
    tk_filter_search_param.ck = tk_filter_token_res.ck;
    tk_filter_search_param.emm = tk_filter_setup_res.emm;
    tk_filter_search_res tk_filter_search_res;
    if (TK_Filter_Search(tk_filter_search_param, tk_filter_search_res) != 0) {
        cout << "call TK_Filter_Search failed!" << endl;
        return -2;
    }
    cout << "TK_Filter_Search success!\n" << endl;

    // call TK_Filter_Resolve
    tk_filter_resolve_param tk_filter_resolve_param;
    tk_filter_resolve_param.w1 = words[0];
    tk_filter_resolve_param.wn = words[words.size()-1];
    tk_filter_resolve_param.ct = tk_filter_search_res.ct;
    tk_filter_resolve_param.mk = tk_filter_setup_res.mk;
    vector<string> tk_filter_ans;
    if (TK_Filter_Resolve(tk_filter_resolve_param, tk_filter_ans) != 0) {
        cout << "call TK_Filter_Resolve failed!" << endl;
        return -2;
    }
    StrVecRemoveTag(tk_filter_ans);
    cout << "TK_Filter_Resolve result: ";
    for (const auto &ans: tk_filter_ans) {
        cout << ans << " ";
    }
    cout << endl;
    cout << "TK_Filter_Resolve success!" << endl;
    return 0;
}

//int main(){
//    string kt = "0101010101010101";
//    string kp = "101010101010101";
//    string l1="l1", l2= "l2", l3="l3", v="id1";
//    string c_l12, c_l13;
//    string ctr = H1(l1, v);
//    string ectr;
//    encrypt(kp, ctr, ectr);
//
//    TK_Enc(kt, l1, l2, v, c_l12);
//    TK_Enc(kt, l1, l3, v, c_l13);
//
//    vector<int> ck3(3);
//    TK_ReGen(kt, l1, l2, l3, 3, ck3);
//
//    string c_l13_plus;
//    TK_ReEnc(ck3, c_l12, c_l13_plus);
//    cout<<c_l13<<endl;
//    cout<<c_l13_plus<<endl;
//
//    string ans;
//    TK_Dec(kt, kp, l1, l3, c_l13_plus, ectr, ans);
//    cout<<"v: "<<ans<<endl;
//}