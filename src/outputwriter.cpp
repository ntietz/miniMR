#include "outputwriter.hpp"

namespace mr
{
    MapperCollector::MapperCollector( SortedDiskCache* cache_
                                    , std::mutex* cacheLock_
                                    )
    {
        cache = cache_;
        cacheLock = cacheLock_;
    }

    void MapperCollector::collect(KeyValuePair& pair)
    {
        cacheLock->lock();
        cache->submit(pair);
        cacheLock->unlock();
    }

    ReducerCollector::ReducerCollector( UnsortedDiskCache* cache_
                                      , std::mutex* cacheLock_
                                      )
    {
        cache = cache_;
        cacheLock = cacheLock_;
    }

    void ReducerCollector::collect(KeyValuePair& pair)
    {
        cacheLock->lock();
        cache->submit(pair);
        cacheLock->unlock();
    }
}
