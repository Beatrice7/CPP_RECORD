#include <iostream>
#include <string>
#include <vector>
#include "MatrixChain.h"
using namespace std;

const int L = 8;
int main(int argc, const char *argv[])
{
    int p[L] = {30, 35, 15, 5, 10, 20, 25, 40};
    MatrixChain matrix_chain(L-1, p);
    matrix_chain.initBothMatrix();
    matrix_chain.lookupChain(1, L-1);
    matrix_chain.printBest();
    matrix_chain.traceChain(1, L-1);
    return 0;
}
