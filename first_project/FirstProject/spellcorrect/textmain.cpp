#include "TextQuery.h"
#include "StringUtils.h"
#include <iostream>
using namespace std;
int main(int argc, const char *argv[])
{
    TextQuery testquery("Chinese.txt", "English.txt");
    testquery.readChineseFile();
    string word;
    while(cin >> word)
    {
        string res = testquery.runQuery(word);
        cout << res;
    }

    testquery.print();

    return 0;
}
