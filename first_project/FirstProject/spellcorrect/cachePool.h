#ifndef CACHE_POOL_H
#define CACHE_POOL_H 

#include "cache.h"
#include <Echo/Conditon.h>
#include <Echo/MutexLock.h>
#include <vector>

class CachePool
{
    public:
        CachePool(){}
        void setSize(size_t size);

    private:
        size_t size_;
        mutable MutexLock mutex_;
        Condition full_;
        std::vector<Cache> caches_;
};

#endif  /*CACHE_POOL_H*/
