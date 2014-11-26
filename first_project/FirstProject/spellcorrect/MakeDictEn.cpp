#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdint.h>
#include "StringUtils.h"
using namespace std;

void readFile(const string &filename, unordered_map<string, int> &wordCount)
{
    ifstream is;
    is.open(filename.c_str());
    string lines;
    while(getline(is, lines))
    {
        stringutils::handleLine(lines);
        string word;
        istringstream line(lines);
        while(line >> word)
        {
            ++wordCount[word];
        }
    }
    is.close();
}


void writeToFile(const string &filename, unordered_map<string, int> &wordCount)
{
    ofstream os;
    os.open(filename.c_str());
    for(auto &item: wordCount){
            os << item.first << " " << item.second << endl; 
    }
    os.close();
}


int main(int argc, const char *argv[])
{
    unordered_map<string, int> wordCount;
    readFile("The_Holy_Bible.txt", wordCount);
    writeToFile("English.txt", wordCount);
    
    return 0;
}
