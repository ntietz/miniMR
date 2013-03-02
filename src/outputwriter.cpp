#include "outputwriter.hpp"

namespace mr
{
    MapperCollector::MapperCollector( std::string baseFilename
                                    , uint64 memLimit
                                    , Comparator comparator
                                    ) : cache(baseFilename, memLimit, comparator)
    {
        // ...
    }

    void MapperCollector::collect(KeyValuePair& pair)
    {
        cache.submit(pair);
    }

    DiskCacheIterator MapperCollector::getIterator()
    {
        return cache.getIterator();
    }
}
