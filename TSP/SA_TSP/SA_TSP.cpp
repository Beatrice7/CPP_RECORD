#include "SA_TSP.h"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;

city cities[CITIES];
double cityMatrix[CITIES][CITIES];

SA_TSP::SA_TSP()
    :markov_(10000), speed_(0.98), currentTemp_(INITAL_TEMP)
{
}

void SA_TSP::initBestUnit()
{
    for(int i = 1; i < CITIES + 1; ++i)
    {
        bestUnit_.path.push_back(i);
    }
    random_shuffle(bestUnit_.path.begin(), bestUnit_.path.end());
    /* 虽然调用了random_shuffle，
     * 但是每次程序开始时第一次shuffle出来的序列都是一样的
     */

    /* for(auto &item: bestUnit_.path){
            cout << item << " ";
    }
    cout << endl;

    bestUnit_.length = lenOfUnit(bestUnit_);
    cout << bestUnit_.length << endl;
    */
}


void SA_TSP::initMatrix(const string &filename)
{
    int i, j;
    ifstream in(filename);
    for(i = 0; i < CITIES; ++i)
    {
        in >> cities[i].id >> cities[i].x >> cities[j].y;
    }
    for(i = 0; i < CITIES; ++i)
    {
        cityMatrix[i][i] = 0;
        for(j = i + 1; j < CITIES; ++j)
        {
            cityMatrix[i][j] = sqrt((cities[i].x - cities[j].x) * (cities[i].x - cities[j].x) + (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
            cityMatrix[j][i] = cityMatrix[i][j];
        }
    }
}


double SA_TSP::lenOfUnit(unit &u)
{
    u.length = 0;
    for(int j = 0; j < CITIES-1; ++j)
    {
        u.length += cityMatrix[u.path[j]][u.path[j+1]];
    }
    u.length += cityMatrix[u.path[CITIES-1]][u.path[0]];

    return u.length;
}


bool SA_TSP::isAccept(unit &best_unit, unit &tmp_unit)
{
    if(best_unit.length > tmp_unit.length)
        return true;
    else
    {
        /*产生0-1之间的随机数用 double(RAND_MAX) */
        double temper = exp((best_unit.length - tmp_unit.length) / (double)currentTemp_);
        double randtemper = rand()/double(RAND_MAX);
        if(temper > randtemper)//必须是'>', 因为temper会越来越小,导致条件越来越不满足，从而趋于稳定
            return true;
    }
    return false;
}

void SA_TSP::genNewUnit(unit &u)
{
/* srand(time(NULL)); 在调用genNewUnit()的外部使用，
 * 否则每次调用都会初始化一次，这样每次都相同了，包括下面的choice也是一样
 * 这与前面的shuffle同理
 */
    int pos1 = rand() % CITIES;
    int pos2 = rand() % CITIES;

    auto ptr = u.path.begin();
    //ensure pos1 != pos2
    while(pos1 == pos2)
        pos2 = rand() % CITIES;
    if(pos1 > pos2)
            swap(pos1, pos2);

    int choice = rand() % 3;
    if(choice == 0) //swap
    {
        //ensure pos1 < pos2
        swap(u.path[pos1], u.path[pos2]);
    }else if(choice == 1) //reverse
    {
        reverse(ptr + pos1, ptr + pos2);
    }else{//rightrotate
        rotate(ptr + pos1, ptr + pos2, ptr + pos2 + 1);
    }

    u.length = lenOfUnit(u);
}

void copy(unit &u1, unit &u2)
{
    u1.path = u2.path;
    u1.length = u2.length;
}

void SA_TSP::simulateAnnealing()
{
    unit tmp_unit;
    copy(tmp_unit, bestUnit_);
    srand(time(NULL)); 
    while(currentTemp_ > MIN_TEMP)
    {
        for(int i = 0; i < markov_; ++i)
        {
            genNewUnit(tmp_unit);
            if(isAccept(bestUnit_, tmp_unit))
            {
                copy(bestUnit_, tmp_unit);
            }
        }
        currentTemp_ *= speed_;
    }
}

void SA_TSP::printBestUnit()
{
    cout << "best path: ";
    for(vector<int>::iterator it = bestUnit_.path.begin(); it != bestUnit_.path.end(); ++it){
        cout << *it << " ";    
    }
    cout << endl << "best_length: " << bestUnit_.length << endl;;
}


