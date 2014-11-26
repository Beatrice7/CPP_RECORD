#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <queue>
#include "TextQuery.h"
#include "StringUtils.h"
using namespace std;

TextQuery::TextQuery(const string &ChineseFilename, const string &EnglishFilename)
    :ChineseFilename_(ChineseFilename), EnglishFilename_(EnglishFilename)
{
    cache_.setCache(100);
}

void TextQuery::readEnglishFile()
{
    ifstream is;
    is.open(EnglishFilename_.c_str()); 
    string line, word;
    int count;
    while(getline(is, line))
    {
        istringstream stream(line);
        stream >> word >> count;
        for(int i = 0; i != word.size(); ++i)
        {
            EnglishClassify_[word[i]][word] = count;
        }
    }
}


void TextQuery::readChineseFile()
{
    ifstream is;
    is.open(ChineseFilename_.c_str()); 
    string line, word;
    int count;
    while(getline(is, line))
    {
        istringstream stream(line);
        stream >> word >> count;
        vector<uint32_t> vec;
        stringutils::parseUTF8String(word, vec);
        for(int i = 0; i != vec.size(); ++i)
        {
            ChineseClassify_[vec[i]][word] = count;
        }
    }
}


void TextQuery::print()
{
     cache_.print() ;  
}

//这里如果runQuery是const版本，则出错；、
//原因是readChineseFile是非const版本；
//由const调用非const会出错
string TextQuery::runQuery(const string &word)
{
    string res;
    vector<uint32_t> vec;
    stringutils::parseUTF8String(word, vec);

    if((res = cache_.get(word)).size() != 0)
    {
        return res;
    }
    else
    {
        if(vec[0] >> 8)
        {
            readChineseFile();
            priority_queue<stringutils::matchWord> ChinesePQ;
            res = ChinesePriorityQueue(word, vec, ChinesePQ);
            cache_.put(word, res);
        }else
        {
            readEnglishFile();
            priority_queue<stringutils::matchWord> EnglishPQ;
            res = EnglishPriorityQueue(word, EnglishPQ);
            cache_.put(word, res);
        }
        return res;
    }
}


string TextQuery::EnglishPriorityQueue(string word, 
        priority_queue<stringutils::matchWord> &EnglishPQ)
{
    for(auto &item: word)
    {
        for(auto &map: EnglishClassify_[item])
        {
            int ret = stringutils::distanceOfString(word, map.first);         
            if(ret <= 3)
            {
                if(ret == 0)
                    return map.first;
                else
                    EnglishPQ.push(stringutils::matchWord(map.first, map.second, ret));
            }
        }
    }
    string res, tmp;
    if(!EnglishPQ.empty())
    {   tmp = EnglishPQ.top().word_ ;
        res += tmp + "\n";
        EnglishPQ.pop();
    }
    for(int i = 1; !EnglishPQ.empty() && i < 10;)
    {
        if(tmp != EnglishPQ.top().word_)
        {
            ++i;
            tmp = EnglishPQ.top().word_;
            res += tmp + "\n";
        }
        EnglishPQ.pop();
    }
    return res;
}



string TextQuery::ChinesePriorityQueue(string word,
        const vector<uint32_t> &vec, 
        priority_queue<stringutils::matchWord> &ChinesePQ)
{
    for(auto &uint32: vec)
    {
        for(auto &map: ChineseClassify_[uint32])
        {
            int ret = stringutils::distanceOfStringCn(word, map.first);
            if(ret <= 3)
            {
                if(ret == 0)
                    return map.first;
                else
                    ChinesePQ.push(stringutils::matchWord(map.first, map.second, ret));
            }
        }
    }
    
    string res, tmp;
    //先输出一个top, 是为了给tmp赋值
    if(!ChinesePQ.empty())
    {   tmp = ChinesePQ.top().word_ ;
        res += tmp + "\n";
        ChinesePQ.pop();
    }

    //当pop之后的top与tmp(即上一个top)不相等时，则输出；
    //这样就避免了重复输出
    for(int i = 1; !ChinesePQ.empty() && i < 10;)
    {
        if(tmp != ChinesePQ.top().word_)
        {
            ++i;
            tmp = ChinesePQ.top().word_;
            res += tmp + "\n";
        }
        ChinesePQ.pop();
    }            
    return res;
}

