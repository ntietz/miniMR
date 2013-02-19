#include "diskcache.hpp"

namespace mr
{
    DiskCache::DiskCache(std::string baseFilename_, uint64 maxSize_ = 1073741824)
    {
        baseFilename = baseFilename_;
        numFiles = 0;
        maxSize = maxSize_;
    }

}

