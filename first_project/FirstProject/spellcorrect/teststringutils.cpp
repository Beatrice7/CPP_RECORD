#include "StringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <fstream>
using namespace std;
int main(int argc, const char *argv[])
{
    string s = "万万没想到";
    string s11 = "abc";
    cout << s.size() << endl;//size:15
    vector<uint32_t> vec;
    stringutils::parseUTF8String(s, vec);
    cout << (vec[0] >> 8) << endl;//又移8位没变0的就是汉字
    stringutils::parseUTF8String(s11, vec);
    cout << (vec[0] >> 8) << endl;//右移8位变0的就是字符串
    for(size_t ix = 0; ix != vec.size(); ++ix)
    {
        printf("%x\n", vec[ix]);
    }

    string s1 = "思维";
    string s2 = "罗辑思维";


    int res = stringutils::distanceOfStringCn(s1, s2);
    cout << res;

    return 0;
}
