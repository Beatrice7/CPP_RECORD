#include <iostream>
#include <algorithm>
#include <string.h>
#define N 1000
using namespace std;
int a1[N], a2[N];
int t1[N], t2[N];
int f1[N], f2[N];
int E[2], X[2];

/*
int f1[N], f2[N];
int E[2] = {10, 20};
int X[2] = {40, 30};
int a1[N] = {50, 100, 1000, 50};
int a2[N] = {200, 200, 200, 65};
int t1[N] = {10, 10, 10};
int t2[N] = {20, 20, 20};
*/

int fastest_way(int);
int main(int argc, const char *argv[])
{
    int n;
    cin >> n;
    
    cin >> E[0] >> E[1] >> X[0] >> X[1];
    memset(a1, 0, sizeof(a1));
    memset(a2, 0, sizeof(a2));
    int i;
    for(i = 0; i<n; ++i)
    {
        cin >> a1[i];
    }
    for(i = 0; i<n; ++i)
    {
        cin >> a2[i];
    }
    for(i = 0; i<n-1; ++i)
    {
        cin >> t1[i];
    }
    for(i = 0; i<n-1; ++i)
    {
        cin >> t2[i];
    }
    
    cout << fastest_way(n);
    
    return 0;
}


int fastest_way(int n)
{
    f1[0] = E[0] + a1[0];
    f2[0] = E[1] + a2[0];

    for(int j = 1; j < n; ++j)
    {
        f1[j] = min(f1[j-1] + a1[j], f2[j-1] + t2[j-1]+ a1[j]);
//        cout << "f1[" << j<< "]  " << f1[j] << endl;
        f2[j] = min(f2[j-1] + a2[j], f1[j-1] + t1[j-1]+ a2[j]);
    }

    return min(f1[n-1] + X[0], f2[n-1] + X[1]);
}
