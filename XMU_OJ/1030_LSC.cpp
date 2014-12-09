#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int> > map;
vector<char> x;
vector<char> y;

void LSClength(int M, int N);
int main(int argc, const char *argv[])
{
    int M, N, i, j;
    cin >> M >> N;
    map.resize(M+1);
    for(i = 0; i <= M; ++i)
    {
        map[i].resize(N + 1);
    }

    for(i = 0; i<= M; ++i){
        for(j = 0; j <= N; ++j)
        {
            map[i][j] = 0;
        }
    }

    char val;
    x.push_back(' ');
    y.push_back(' ');
    for(i = 0; i < M; ++i){
        cin >> val;
        x.push_back(val);
    }
    for(i = 0; i < N; ++i){
        cin >> val;
        y.push_back(val);
    }
    /*for(vector<char>::iterator it = x.begin(); it != x.end(); ++it){
        cout << *it << " ";    
    }
    cout << endl;
    for(vector<char>::iterator it = y.begin(); it != y.end(); ++it){
        cout << *it << " ";    
    }
    cout << endl;
    */
    LSClength(M, N);
    cout << map[M][N];
    
    return 0;
}

void LSClength(int M, int N)
{
    int i, j;
    for(i = 1; i <= M; ++i)
    {
        for(j = 1; j <= N; ++j)
        {
            if(x[i] != y[j])
                map[i][j] = max(map[i-1][j], map[i][j-1]);
            else
                map[i][j] = map[i-1][j-1] + 1;
        }
    }
}
