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

    // MapperCollector is not synchronized, since each should only be used by one mapper.
    class MapperCollector : public OutputCollector
    {
      public:
        // TODO constructor
        MapperCollector(std::string baseFilename, uint64 memLimit, Comparator comparator);
        void collect(KeyValuePair& pair);

        DiskCacheIterator getIterator();
      private:
        SortedDiskCache cache;
    };

    class ReducerCollector : public OutputCollector
    {
      public:
        void collect(KeyValuePair& pair);
    };
}

#endif
