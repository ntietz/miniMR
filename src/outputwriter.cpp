#include "outputwriter.hpp"

namespace mr
{
    /*
    MapperCollector::MapperCollector( std::string baseFilename
                                    , uint64 memLimit
                                    , Comparator comparator
                                    ) : cache(baseFilename, memLimit, comparator)
    {
        // ...
    }
    */

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

    /*
    DiskCacheIterator MapperCollector::getIterator()
    {
        return cache.getIterator();
    }
    */
}
