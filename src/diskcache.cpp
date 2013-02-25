#include "diskcache.hpp"
#include "keyvaluepair.hpp"
#include "types.hpp"

#include <string>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>

namespace mr
{
    std::string generateFilename(std::string baseFilename, uint32 fileNumber)
    {
        std::stringstream filenameCreator;
        filenameCreator << baseFilename;
        filenameCreator << fileNumber;
        return filenameCreator.str();
    }

    UnsortedDiskCache::UnsortedDiskCache(std::string baseFilename_, uint64 maxSize_ = 1073741824)
    {
        baseFilename = baseFilename_;
        numFiles = 0;
        maxSize = maxSize_;

        size = 0;
    }

    UnsortedDiskCache::~UnsortedDiskCache()
    {
        for (uint32 fileNumber = 0; fileNumber < numFiles; ++fileNumber)
        {
            std::string filename = generateFilename(baseFilename, fileNumber);
            std::remove(filename.c_str());
        }
    }

    void UnsortedDiskCache::submit(bytelist& key, bytelist& value)
    {
        KeyValuePair pair(key, value);
        submit(pair);
    }

    void UnsortedDiskCache::submit(KeyValuePair& pair)
    {
        size += pair.key.size();
        size += pair.value.size();
        contents.push_back(pair);

        if (size > maxSize)
        {
            flush();
        }
    }

    void UnsortedDiskCache::flush()
    {
        int fileNumber = numFiles;
        std::string filename = generateFilename(baseFilename, fileNumber);

        std::ofstream out(filename.c_str());
        uint32 numberOfRecords = contents.size();
        out.write((char*) &numberOfRecords, sizeof(uint32));

        for (int index = 0; index < contents.size(); ++index)
        {
            uint32 keySize = contents[index].key.size();
            out.write((char*) &keySize, sizeof(uint32));
            out.write(contents[index].key.data(), keySize);

            uint32 valueSize = contents[index].value.size();
            out.write((char*) &valueSize, sizeof(uint32));
            out.write(contents[index].value.data(), valueSize);
        }

        ++numFiles;
        contents.clear();
        size = 0;
    }

    UnsortedDiskCache::Iterator UnsortedDiskCache::getIterator()
    {
        if (size > 0)
        {
            flush();
        }

        return UnsortedDiskCache::Iterator(baseFilename, numFiles, maxSize);
    }

    UnsortedDiskCacheIterator::UnsortedDiskCacheIterator( std::string baseFilename_
                                                        , uint32 numFiles_
                                                        , uint64 maxSize_
                                                        )
    {
        baseFilename = baseFilename_;
        numFiles = numFiles_;
        maxSize = maxSize_;
        size = 0;
        currentFile = 0;
        startedReading = false;
        numRemaining = 0;
    }

    bool UnsortedDiskCacheIterator::hasNext()
    {
        if (contents.size() == 0)
        {
            populateCache();
        }

        if (contents.size() > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    KeyValuePair UnsortedDiskCacheIterator::getNext()
    {
        if (!hasNext())
        {
            throw "EMPTY CACHE"; // TODO FIXME throw a real exception
        }

        KeyValuePair result = contents.back();
        contents.pop_back();
        size -= (result.key.size() + result.value.size());
        return result;
    }

    void UnsortedDiskCacheIterator::populateCache()
    {
        if (currentFile < numFiles)
        {
            if (!startedReading)
            {
                // TODO read the number remaining
            }

            // TODO read until the cache is full
        }
    }
}

