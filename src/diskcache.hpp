#ifndef _DISKCACHE_HPP_
#define _DISKCACHE_HPP_

#include <string>
#include "types.hpp"

namespace mr
{
    class DiskCache
    {
      public:
        DiskCache(std::string baseFilename_, unsigned int maxSize_);

        void submitByteList(bytelist);
        void flush();

        // TODO iterator

      private:
        std::string baseFilename;
        unsigned int numFiles;
        unsigned int maxSize;
    };
}

#endif
