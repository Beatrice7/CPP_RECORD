#ifndef MATRIX_CHAIN_H_
#define MATRIX_CHAIN_H_ 
#include <iostream>
#include <vector>

class MatrixChain
{
    public:
        MatrixChain(int nMatrix, int value[])
            :nMatrix_(nMatrix), value_(value)
        {}
        void initBothMatrix();
        int lookupChain(int i, int j);
        void traceChain(int i, int j);
        void printBest();
        ~MatrixChain(){}


    private:
        int* value_; //to store each Matrix's column and the first Matrix'srow
        int nMatrix_; 
        int minCount_; //final min result
        std::vector<std::vector<int> > m_; //to store min result between 'Matrix_i' and 'Matrix_j'
        std::vector<std::vector<int> > s_; //to store the segment position, s[i][j]=k

};

#endif  /*MATRIX_CHAIN_H_*/
