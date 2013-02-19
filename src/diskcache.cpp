#include "diskcache.hpp"

namespace mr
{
    UnsortedDiskCache::UnsortedDiskCache(std::string baseFilename_, uint64 maxSize_ = 1073741824)
    {
        baseFilename = baseFilename_;
        numFiles = 0;
        maxSize = maxSize_;

        size = 0;
    }

    void UnsortedDiskCache::submit(bytelist value)
    {
        size += value.size();
        contents.push_back(value);

        if (size > maxSize)
        {
            flush();
        }
    }

    void UnsortedDiskCache::flush()
    {
        // TODO write to new file
        // TODO increment file count
        // TODO reset size to 0
    }
}

