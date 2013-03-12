#include "outputwriter.hpp"

namespace mr
{
    OutputCollector::~OutputCollector()
    {
        // ...
    }

    MapperCollector::MapperCollector( SortedDiskCache* cache_
                                    , std::atomic_flag* cacheLock_
                                    )
    {
        cache = cache_;
        cacheLock = cacheLock_;
    }

    void MapperCollector::collect(KeyValuePair& pair)
    {
        //cacheLock->lock();
        while (cacheLock->test_and_set()) { }
        cache->submit(pair);
        cacheLock->clear();
        //cacheLock->unlock();
    }

    ReducerCollector::ReducerCollector( UnsortedDiskCache* cache_
                                      , std::atomic_flag* cacheLock_
                                      )
    {
        cache = cache_;
        cacheLock = cacheLock_;
    }

    void ReducerCollector::collect(KeyValuePair& pair)
    {
        //cacheLock->lock();
        while (cacheLock->test_and_set()) { }
        cache->submit(pair);
        cacheLock->clear();
        //cacheLock->unlock();
    }
}
