#include <iostream>
#include <string>
#include <vector>
#include "MatrixChain.h"

using namespace std;
string result = "(A1A2A3A4A5A6)";

void MatrixChain::initBothMatrix()
{
    m_.resize(nMatrix_ + 1);
    s_.resize(nMatrix_ + 1);
    for(int i = 0; i <= nMatrix_; ++i)
    {
        m_[i].resize(nMatrix_ + 1);
        s_[i].resize(nMatrix_ + 1);
    }

    for(int i = 0; i <= nMatrix_; ++i)
    {
        for(int j = 0; j <= nMatrix_; ++j)
        {
            m_[i][j] = 0;
            s_[i][j] = 0;
        }
    }
}



int MatrixChain::lookupChain(int i, int j)
{
    if(m_[i][j] > 0)
        return m_[i][j];
    if(i == j)
        return 0;
    int u = lookupChain(i, i) + lookupChain(i + 1, j) + *(value_+i-1) * *(value_+i) * *(value_+j);
    s_[i][j] = i;

    for(int k = i + 1; k < j; ++k)
    {
        int t = lookupChain(i, k) + lookupChain(k + 1, j) + *(value_+i-1) * *(value_+k) * *(value_+j);
        if(t < u)
        {
            u = t;
            s_[i][j] = k;
        }
    }
    m_[i][j] = u;
    return u;
}

void MatrixChain::traceChain(int i, int j)
{
    if(i == j){
        cout << "A"<< i;
    }else if(i+1 == j){
        cout <<"(A" << i << "A" << j << ")";
    }else{
        cout << "(";
        traceChain(i, s_[i][j]);
        traceChain(s_[i][j] + 1, j);
        cout << ")";
    }
}

void MatrixChain::printBest()
{
    cout << "m_: " << endl;
    for(int i = 0; i < nMatrix_; ++i)
    {
        for(int j = 0; j<= nMatrix_; ++j)
            cout << m_[i][j] << " ";
        cout << endl;
    }
    cout << "s_: " << endl;
    for(int i = 0; i <= nMatrix_; ++i)
    {
        for(int j = 0; j<= nMatrix_; ++j)
            cout << s_[i][j] << " ";
        cout << endl;
    }
    cout << "min multiply count: " << m_[1][nMatrix_] << endl;
}
