#include <iostream>
#include <string>
#include <vector>
#include "MatrixChain.h"
using namespace std;

const int L = 7;
//A1 30*35 A2 35*15 A3 15*5 A4 5*10 A5 10*20 A6 20*25
int main(int argc, const char *argv[])
{
    //p中记录的是所有矩阵的行与最后一个矩阵的列对应的value
    int p[L] = {30, 35, 15, 5, 10, 20, 25};
    MatrixChain matrix_chain(L-1, p);
    matrix_chain.initBothMatrix();
    matrix_chain.lookupChain(1, L-1);
    matrix_chain.printBest();
    matrix_chain.traceChain(1, L-1);

    return 0;
}
