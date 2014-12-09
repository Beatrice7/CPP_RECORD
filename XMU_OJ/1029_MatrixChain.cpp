#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#define N 200
using namespace std;

int m[N][N], s[N][N];
int vec[N+1];

int lookup(int, int);

int main(int argc, const char *argv[])
{
    int n, val;
    cin >> n;
    memset(vec, 0, sizeof(vec));
    for(int i = 0; i <= n; ++i)
    {
        cin >> val;
        vec[i] = val;
    }
    
    memset(m, 0, sizeof(m));
    memset(s, 0, sizeof(s));
    cout << lookup(1, n);


    return 0;
}

int lookup(int i, int j)
{
    if(i == j)
        return 0;

    if(m[i][j] > 0)
        return m[i][j];

    int u = lookup(i,i) + lookup(i+1, j) + vec[i-1]*vec[i]*vec[j];
    s[i][j] = i;

    for(int k = i+1; k < j; k++){
        int t = lookup(i, k) + lookup(k+1, j) + vec[i-1]*vec[k]*vec[j];
        if(t < u){
            u = t;
            s[i][j] = k;
        }
    }
    m[i][j] = u;
    return u;
}
