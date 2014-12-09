#include <iostream>
#include <string.h>
#include <algorithm>
#define MAX 1000
using namespace std;
int map[MAX][MAX];
int dp[MAX][MAX];

int main(int argc, const char *argv[])
{
    int R;
    cin >> R;
    memset(dp, 0, sizeof(dp));
    
    for(int i = 1; i <= R; ++i)
    {
        for(int j= 1; j <= i; ++j)
        {
            cin >> map[i][j];
        }
    }
    for(int i = R; i >= 1; --i)
    {
        for(int j = 1; j <= i; ++j)
            dp[i][j] = map[i][j] + max(dp[i+1][j], dp[i+1][j+1]);
    }
    cout << dp[1][1];

    return 0;
}
