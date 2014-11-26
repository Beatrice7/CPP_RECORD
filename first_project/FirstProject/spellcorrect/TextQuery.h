#ifndef TEXTQUERY_H
#define TEXTQUERY_H
#include "StringUtils.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
#include "cache.hpp"

class TextQuery
{
    public:
        TextQuery(const std::string &ChineseFilename, const std::string &EnglishFilename);
        void readEnglishFile();
        void readChineseFile();
        std::string ChinesePriorityQueue(std::string word, 
                const std::vector<uint32_t> &vec, 
                std::priority_queue<stringutils::matchWord> &ChinesePQ);
        std::string EnglishPriorityQueue(std::string word, 
                std::priority_queue<stringutils::matchWord> &EnglishPQ);
        std::string runQuery(const std::string &word);
        void print();

    private:
       std::string ChineseFilename_;
       std::string EnglishFilename_;
       std::unordered_map<char, std::unordered_map<std::string, int> > EnglishClassify_;
       std::unordered_map<uint32_t, std::unordered_map<std::string, int> > ChineseClassify_;
       Cache<std::string, std::string> cache_;
};

#endif  /*TEXTQUERY_H*/
