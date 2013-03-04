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
        MapperCollector(SortedDiskCache* cache_, std::mutex* cacheLock_);
        void collect(KeyValuePair& pair);

      private:
        SortedDiskCache* cache;
        std::mutex* cacheLock;
    };

    class ReducerCollector : public OutputCollector
    {
      public:
        ReducerCollector(UnsortedDiskCache* cache_, std::mutex* cacheLock_);
        void collect(KeyValuePair& pair);

      private:
        UnsortedDiskCache* cache;
        std::mutex* cacheLock;
    };
}

#endif
