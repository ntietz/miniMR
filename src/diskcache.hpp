#ifndef _DISKCACHE_HPP_
#define _DISKCACHE_HPP_

#include "keyvaluepair.hpp"
#include "types.hpp"

#include <string>
#include <vector>
#include <fstream>

namespace mr
{
    class UnsortedDiskCache;
    class UnsortedDiskCacheIterator;
    class SortedDiskCache;
    class SortedDiskCacheIterator;

    std::string generateFilename(std::string baseFilename, uint32 fileNumber);

    // Note: this currently only supports writing out the data then reading in
    // the data, then the container is empty and should not be reused.
    class UnsortedDiskCache
    {
      public:
        typedef UnsortedDiskCacheIterator Iterator;

        UnsortedDiskCache(std::string baseFilename_, uint64 maxSize_);
        ~UnsortedDiskCache();

        void submit(bytelist&, bytelist&);
        void submit(KeyValuePair&);
        void flush();

        Iterator getIterator();

      private:
        std::string baseFilename;
        uint32 numFiles;
        uint64 maxSize;

        std::vector<KeyValuePair> contents;
        uint64 size;
    };

    class UnsortedDiskCacheIterator
    {
      public:
        UnsortedDiskCacheIterator( std::string baseFilename_
                                 , uint32 numFiles_
                                 , uint64 maxSize_
                                 );
        ~UnsortedDiskCacheIterator();

        bool hasNext();
        KeyValuePair getNext();

      private:
        std::string baseFilename;
        uint32 numFiles;
        uint64 maxSize;

        std::vector<KeyValuePair> contents;
        uint64 size;

        void populateCache();
        std::ifstream* in;
        uint32 currentFile;
        bool startedReading;
        uint32 numRemaining;
    };

    class SortedDiskCache
    {
      public:
        // TODO constructor with compare function

        void submit(bytelist);
        void flush();
    };
}

#endif
