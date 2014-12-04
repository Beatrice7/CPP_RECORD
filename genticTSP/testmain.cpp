#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "GenticTSP.h"
using namespace std;


int main(int argc, const char *argv[])
{
    GenticTSP genticTsp;
    genticTsp.initMatrix("city.txt");
    genticTsp.initGroup();
    genticTsp.evoluteGroup();
    genticTsp.selectGroup();
    genticTsp.printBestUnit ();

    return 0;
}
