#ifndef _OUTPUTWRITER_HPP_
#define _OUTPUTWRITER_HPP_

#include <mutex>
#include "keyvaluepair.hpp"
#include "diskcache.hpp"

namespace mr
{
    class OutputCollector
    {
      public:
        virtual void collect(KeyValuePair&) = 0;
    };

    // MapperCollector is synchronized so that all the mappers output into the same cache
    class MapperCollector : public OutputCollector
    {
      public:
        // TODO constructor
        //MapperCollector(std::string baseFilename, uint64 memLimit, Comparator comparator);
        MapperCollector(SortedDiskCache* cache_, std::mutex* cacheLock_);
        void collect(KeyValuePair& pair);

        //DiskCacheIterator getIterator();
      private:
        SortedDiskCache* cache;
        std::mutex* cacheLock;
    };

    class ReducerCollector : public OutputCollector
    {
      public:
        void collect(KeyValuePair& pair);
    };
}

#endif
