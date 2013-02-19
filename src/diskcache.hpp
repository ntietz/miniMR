#ifndef _DISKCACHE_HPP_
#define _DISKCACHE_HPP_

#include <string>
#include <vector>
#include "types.hpp"

namespace mr
{
    class UnsortedDiskCache
    {
      public:
        UnsortedDiskCache(std::string baseFilename_, uint64 maxSize_);

        void submit(bytelist);
        void flush();

        // TODO iterator

      private:
        std::string baseFilename;
        uint32 numFiles;
        uint64 maxSize;

        std::vector<bytelist> contents;
        uint64 size;
    };

    class SortedDiskCache
    {
        // TODO constructor with compare function

    };
}

#endif
