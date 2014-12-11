#include <iostream>
#include <string.h>
#include <string>
//#include <stdio.h>
using namespace std;
const int MAX = 210;
const int inf = 0x7fffffff;
char str[MAX];
int len;
int d[MAX][MAX];
int pos[MAX][MAX]; //记录添加括号的位置

void dp(){
    for(int k = 1; k < len; ++k)
    {
        for(int i = 0; i + k < len; ++i)
        {
            int j = i + k;
            d[i][j] = inf;
            
            if((str[i] == '(' && str[j] == ')') || (str[i] == '[' && str[j] == ']'))
            {
                d[i][j] = d[i+1][j-1];
            }

            for(int mid = i; mid < j; ++mid)
            {
                if(d[i][j] > d[i][mid] + d[mid+1][j]){
                    d[i][j] = d[i][mid] + d[mid+1][j];
                    pos[i][j] = mid;
                }
            }

        }
    }
}

void print(int i, int j)
{
    if(i > j)
        return ;
    if(i == j)
    {
        if(str[i] == ')' || str[i] == '(')
            cout << "()";
        else
            cout << "[]";
    }
    else if(pos[i][j] == -1)
    {
        cout << str[i];
        print(i+1, j-1);
        cout << str[j];
    }else{
        print(i, pos[i][j]);
        print(pos[i][j] + 1, j);
    }

}


int main(int argc, const char *argv[])
{
   // fscanf(stdin, "%s", str);
    gets(str);
    memset(d, 0, sizeof(d));
    memset(pos, -1, sizeof(pos));
    len = strlen(str);
    for(int i = 0; i < len; ++i) d[i][i] = 1, pos[i][i] = 0;

    dp();
    //cout << d[0][len-1];
    print(0, len-1);

    return 0;
}
