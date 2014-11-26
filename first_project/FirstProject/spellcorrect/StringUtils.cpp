#include "StringUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
using namespace std;

namespace stringutils
{
    int minValue(int a, int b, int c)
    {
        int t = a <= b ? a : b;
        return t <=c ? t : c;
    }

    int distanceOfString(const std::string &strA, const std::string &strB)
    {

        int lenA = (int)strA.length()+1;
        int lenB = (int)strB.length()+1;

        int **c = new int*[lenA];
        for(int i = 0; i < lenA; i++)
            c[i] = new int[lenB];

        for(int i = 0; i < lenA; i++) c[i][0] = i;
        for(int j = 0; j < lenB; j++) c[0][j] = j;

        for(int i = 1; i < lenA; i++)
        {
            for(int j = 1; j < lenB; j++)
            {
                if(strB[j-1] == strA[i-1])
                    c[i][j] = c[i-1][j-1];
                else
                    c[i][j] = minValue(c[i][j-1], c[i-1][j], c[i-1][j-1]) + 1;
            }
        }

        int ret =  c[lenA-1][lenB-1];

        for(int i = 0; i < lenA; i++)
            delete [] c[i];
        delete []c;

        return ret;
    }
    
    void handleLine(std::string &str)
    {
        for(auto &item: str){
                item = (ispunct(item) ? ' ' : item);
                item = (isupper(item) ? tolower(item) : item);
                if(item > '0' && item < '9')
                    item = ' ';
        }
    }

    int getLenOfUTF8(unsigned char c)
    {
        int cnt = 0;
        while(c & (1 << (7 - cnt)))
            ++cnt;
        return cnt;
    }

    void parseUTF8String(const std::string &s, vector<uint32_t> &vec) 
    {
        vec.clear();
        for(string::size_type ix = 0; ix != s.size(); ++ix)
        {
            int len = getLenOfUTF8(s[ix]);
            uint32_t t = (unsigned char)s[ix];
            if(len > 1)
            {
                --len;
                while(len--)
                {
                    t = (t << 8) + (unsigned char)s[++ix];
                }
            }
            vec.push_back(t);
        }
    }

    int distanceOfUint32(const vector<uint32_t> &w1, const vector<uint32_t> &w2)
    {
        int len1 = w1.size();
        int len2 = w2.size();
        int memo[100][100];
        memset(memo, 0x00, 100 * 100 * sizeof(int));
        for(int i = 1; i <= len1; ++i)
        {
            memo[i][0] = i;
        }
        for(int j = 1; j <= len2; ++j)
        {
            memo[0][j] = j;
        }
        
        for(int i = 1; i <= len1; ++i)
        {
            for(int j = 1; j <= len2; ++j)
            {
                if(w1[i-1] == w2[j-1])
                {
                    memo[i][j] = memo[i-1][j-1];
                }else
                {
                    memo[i][j] = minValue(memo[i-1][j-1], memo[i][j-1], memo[i-1][j]) + 1;
                }
            }
        }
       // std::cout << memo[len1][len2]<<"____" << std::endl;
        return memo[len1][len2];
    }

    int distanceOfStringCn(const std::string &a, const std::string &b)
    {
        vector<uint32_t> w1, w2;
        parseUTF8String(a, w1);
        parseUTF8String(b, w2);

        return distanceOfUint32(w1, w2);
    }
}

