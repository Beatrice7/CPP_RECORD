#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "MixSegment.hpp"
#include <string>
//#include "MPSegment.hpp"
//#include "HMMSegment.hpp"
//#include "FullSegment.hpp"
//#include "QuerySegment.hpp"

using namespace CppJieba;

const char * const dict_path =  "dict/jieba.dict.utf8";
const char * const model_path = "dict/hmm_model.utf8";

//const char * const test_lines = "我来到南京市长江大桥";

int main(int argc, char ** argv)
{
    /*segment init may take a few seconds.*/
    MixSegment segment(dict_path, model_path);
    ifstream is;
    is.open("redandwhite.txt");
    string test_lines;
    map<string, int> ChineseDict;
    while(getline(is, test_lines))
    {
        vector<string> words;
        segment.cut(test_lines, words);
        for(auto &item: words){
                ++ChineseDict[item];
        }
    }
    is.close();
    
    ofstream os;
    os.open("Chinese.txt");
    for(auto &item: ChineseDict){
            os << item.first << " " << item.second << endl;
    }
    os.close();
    return 0;
}
