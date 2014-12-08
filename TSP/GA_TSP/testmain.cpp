#include "SA_TSP.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main(int argc, const char *argv[])
{
    SA_TSP saTsp;
    saTsp.initMatrix("city.txt");
    saTsp.initBestUnit();
    saTsp.simulateAnnealing();
    saTsp.printBestUnit();
    return 0;
}

