#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "cache.hpp"
using namespace std;

int main(int argc, const char *argv[])
{
    unordered_map<int, int> map;
    map[9]= 999;
    cout<<map[9]<<endl;
    cout<<map[10]<<endl;
    Cache<int, string> lru_cache;
    lru_cache.setCache(100);
    lru_cache.put(1, "one");
    lru_cache.put(2, "two");
    lru_cache.print();
    /*
    cout<<lru_cache.get(1)<<endl;
    if(lru_cache.get(2) == "")
        lru_cache.put(2, "two");
    cout<<lru_cache.get(2);
    */
    return 0;
}
