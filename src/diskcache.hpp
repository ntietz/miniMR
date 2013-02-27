#ifndef _DISKCACHE_HPP_
#define _DISKCACHE_HPP_

#include "keyvaluepair.hpp"
#include "types.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <functional>

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

        virtual void submit(bytelist&, bytelist&);
        virtual void submit(KeyValuePair&);
        virtual void flush();

        Iterator getIterator();

      protected:
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

      protected:
        std::string baseFilename;
        uint32 numFiles;
        uint64 maxSize;

        std::vector<KeyValuePair> contents;
        uint64 size;

        virtual void populateCache();
      private:
        std::ifstream* in;
        uint32 currentFile;
        bool startedReading;
        uint32 numRemaining;
    };

    typedef std::function<bool(const KeyValuePair&, const KeyValuePair&)> Comparator;

    class SortedDiskCache : public UnsortedDiskCache
    {
      public:
        typedef SortedDiskCacheIterator Iterator;
        SortedDiskCache(std::string baseFilename_, uint64 maxSize_, Comparator comparator_);
        ~SortedDiskCache();

        void flush();

        Iterator getIterator();
      protected:
        Comparator comparator;
    };

    class SortedDiskCacheIterator : public UnsortedDiskCacheIterator
    {
      public:
        SortedDiskCacheIterator( std::string baseFilename_
                               , uint32 numFiles_
                               , uint64 maxSize_
                               , Comparator comparator_
                               );
        ~SortedDiskCacheIterator();

      protected:
        Comparator comparator;
        void populateCache();
        std::ifstream* in;
        uint32* numRemaining;
    };
}

#endif
