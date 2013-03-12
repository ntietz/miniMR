#ifndef _OUTPUTWRITER_HPP_
#define _OUTPUTWRITER_HPP_

#include <mutex>
#include <atomic>
#include "keyvaluepair.hpp"
#include "diskcache.hpp"

namespace mr
{
    class OutputCollector
    {
      public:
        virtual ~OutputCollector();
        virtual void collect(KeyValuePair&) = 0;
    };

    // MapperCollector is synchronized so that all the mappers output into the same cache
    class MapperCollector : public OutputCollector
    {
      public:
        MapperCollector(SortedDiskCache* cache_, std::atomic_flag* cacheLock_);
        void collect(KeyValuePair& pair);

      private:
        SortedDiskCache* cache;
        std::atomic_flag* cacheLock;
    };

    class ReducerCollector : public OutputCollector
    {
      public:
        ReducerCollector(UnsortedDiskCache* cache_, std::atomic_flag* cacheLock_);
        void collect(KeyValuePair& pair);

      private:
        UnsortedDiskCache* cache;
        std::atomic_flag* cacheLock;
    };
}

#endif
