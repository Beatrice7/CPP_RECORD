#include "GenticTSP.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <math.h>
#include <time.h>
#include <string.h>

using namespace std;

city cities[CITIES];
unit group[UNITS];
double cityMatrix[CITIES][CITIES];

GenticTSP::GenticTSP()
    :pm_(0.2), ps_(0.8)
{

}

GenticTSP::~GenticTSP()
{

}

void GenticTSP::initMatrix(const string &filename)
{
    int i, j;
    ifstream in(filename);
    for(i = 0; i < CITIES; ++i)
    {

        in >> cities[i].id >> cities[i].x >> cities[i].y;
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


//calculate the path-lenght of each path
double GenticTSP::lenOfUnit(unit &unit)
{
    unit.length = 0;
    for(int j = 0; j < CITIES - 1; ++j)
    {
        unit.length += cityMatrix[unit.path[j]][unit.path[j + 1]];
    }

    //回到起始城市
    unit.length += cityMatrix[unit.path[0] ][unit.path[CITIES - 1] ];

    return unit.length;
}


//init group of various paths
void GenticTSP::initGroup()
{
    vector<int> tmp;
    for(int i = 0; i < CITIES; ++i)
        tmp.push_back(i);

    for(int i = 0; i < UNITS; ++i)
    {
        random_shuffle(tmp.begin(),tmp.end());
        group[i].path = tmp;
        group[i].length = lenOfUnit(group[i]);
    }
}

//for test
void printlist(list<int> &path)
{
    for(list<int>::iterator it = path.begin(); it != path.end(); ++it){
        cout << *it << " ";    
    }
    cout << endl;
}

void rightRotateList(list<int> &path, list<int>::iterator ptr, list<int>::iterator res)
{
    path.insert(ptr, res, path.end());
    path.erase(res, path.end());
}


void GenticTSP::selectGroup()
{
    int selected = UNITS * ps_;
    int non_selected = UNITS - selected; //防止取整丢失，所以直接采用减去selected
    for(int i = 0; i < UNITS; ++i)
    {
        group[i].length = lenOfUnit(group[i]);
    }
    sort(group, group + UNITS);

    /*群体规模保持不变
     *被淘汰的个体数目为non_selected
     *用被选择的前non_seleted个个体替代之
     */
    for(int i = 0; i < non_selected; ++i)
    {
        group[UNITS-1-i].path =  group[i].path;
        group[UNITS-1-i].length =  group[i].length;

       /* WARNING:
        * error:memcpy(&group[UNITS-1-i], &group[i], sizeof(unit)); 
        * 不能用memcpy
        * 因为unit是自定义的结构体
        * 其中的vector类型也不是原生的数据类型
        * 如若使用，需要自定义构造函数
        */
    }
}


void GenticTSP::evoluteGroup()
{
    srand(time(NULL)); //srand一次即可！
    for(int i = 0; i < MAX_GEN; ++i)
    {
        selectGroup();

        for(int j = 0; j < UNITS-2;)
        {
            crossUnits(group[j], group[j+1], group[j+2]);
            j += 2;
        }

        mutateGroup();
    }

}


void GenticTSP::mutateGroup()
{
//    srand(time(NULL));
    int num;
    int pos1, pos2;
    int tmp;
    int sum = UNITS * pm_; //变异的总数= 群体数 * 变异概率
    while(sum--)
    {
        //随机选取一个发生变异的unit
        num = rand() % UNITS;
        //随机选取两个path上变异的城市，采用交换两个城市的方法进行变异
        pos1 = rand() % CITIES;
        pos2 = rand() % CITIES;
//        cout << "pos1:" << pos1 << "pos2: " << pos2 << endl;

        //如果相同则不能算是变异，这里用while确保变异
        while(pos1 == pos2)
            pos2 = rand() % CITIES;

        tmp = group[num].path[pos1];
        group[num].path[pos1] = group[num].path[pos2];
        group[num].path[pos2] = tmp;

        //更新该变异后path的长度
        lenOfUnit(group[num]);      
    }
}




void GenticTSP::crossUnits(unit &u1, unit &u2, unit &u3)
{
    //将路径存储在list类型的容器中便于后面的插入操作
    list<int> path1(u1.path.begin(), u1.path.end());
    list<int> path2(u2.path.begin(), u2.path.end());
    list<int> path3(u3.path.begin(), u3.path.end());
    //随机产生起始城市的id， 并将位置默认为第一个位置
   // srand((int)time(0));
    int cityID = rand() % CITIES;
    list<int>::iterator res1, res2, res3;
    res1 = find(path1.begin(), path1.end(), cityID);
    res2 = find(path2.begin(), path2.end(), cityID);
    res3 = find(path3.begin(), path3.end(), cityID);
    if(res1 == path1.end() || res2 == path2.end() || res3 == path3.end())
    {
        cout << "city not found" << endl;
    }
    
    //将以随机选取的城市及其后面的城市右旋到序列首部
    rightRotateList(path1, path1.begin(), res1);
    rightRotateList(path2, path2.begin(), res2);
    rightRotateList(path3, path3.begin(), res3);

    list<int>::iterator ptr1, ptr2, ptr3;
    ptr1 = path1.begin();
    ptr2 = path2.begin();
    ptr3 = path3.begin();

    /*CITIES个城市
     *第一个城市在循环体外已经确定
     *循环体内只要再执行CITIES - 2 次即可
     *因为当执行倒数第二次后，最后一个也就是仅剩的一个也无需再循环了
     */
    for(int k = 1; k < CITIES-1; ++k)
    {
      //  cout << "______" << k<< "______" << endl;
        int h1 = *ptr1;
        double len1 = cityMatrix[h1][*(++ptr1)];
        int h2 = *ptr2;
        double len2 = cityMatrix[h2][*(++ptr2)];
        int h3 = *ptr3;
        double len3 = cityMatrix[h3][*(++ptr3)];

        //找出前两个城市距离最小的那个path中得第一个城市的编号
        double len = (len1 <= len2) ? len1 : len2;
        len = (len < len3) ? len : len3;
        if(len == len1){
            cityID = *ptr1;
        }else if(len == len2){
            cityID = *ptr2;
        }else{
            cityID = *ptr3;
        }

        //与当前城市距离最小的下一个城市编号为cityID
        res1 = find(ptr1, path1.end(), cityID);
        res2 = find(ptr2, path2.end(), cityID);
        res3 = find(ptr3, path3.end(), cityID);
        if(res1 == path1.end() || res2 == path2.end() || res3 == path3.end())
        {
            cout << "city not found in loop" << endl;
        }
        rightRotateList(path1, ptr1, res1);
        rightRotateList(path2, ptr2, res2);
        rightRotateList(path3, ptr3, res3);

        //指针失效，需要重新定位
        ptr1 = find(path1.begin(), path1.end(), cityID);
        ptr2 = find(path2.begin(), path2.end(), cityID);
        ptr3 = find(path3.begin(), path3.end(), cityID);
    }


    /*为了避免三个路径相同，所以这里在不改变路径的情况下替换起点城市，有利于接下来的交叉*/
    cityID = (rand() % CITIES);
    res2 = find(path2.begin(), path2.end(), cityID);
    rightRotateList(path2, path2.begin(), res2);

    cityID = (rand() % CITIES);
    res3 = find(path3.begin(), path3.end(), cityID);
    rightRotateList(path3, path3.begin(), res3);

    //交叉完之后再赋值给vector类型
    u1.path.assign(path1.begin(), path1.end());
    u2.path.assign(path2.begin(), path2.end());
    u3.path.assign(path3.begin(), path3.end());
}
    
    
//print top5 paths with length
void GenticTSP::printBestUnit()
{
    for(int i = 0; i < 5; ++i)
    {
        cout << "Path" << i << ": ";
        for(vector<int>::iterator it = group[i].path.begin(); it != group[i].path.end(); ++it){
            cout << *it << " ";    
        }
        cout << endl << "length: " << group[i].length << endl; 
    }
}




