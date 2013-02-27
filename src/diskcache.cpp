#include "diskcache.hpp"
#include "keyvaluepair.hpp"
#include "types.hpp"

#include <string>
#include <cstdio>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace mr
{
    std::string generateFilename(std::string baseFilename, uint32 fileNumber)
    {
        std::stringstream filenameCreator;
        filenameCreator << baseFilename;
        filenameCreator << fileNumber;
        return filenameCreator.str();
    }

    UnsortedDiskCache::UnsortedDiskCache(std::string baseFilename_, uint64 maxSize_)
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

        for (uint32 index = 0; index < contents.size(); ++index)
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

        return UnsortedDiskCacheIterator(baseFilename, numFiles, maxSize);
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
        in = new std::ifstream();
    }

    UnsortedDiskCacheIterator::~UnsortedDiskCacheIterator()
    {
        delete in;
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
        while (currentFile < numFiles)
        {
            if (!startedReading)
            {
                std::string filename = generateFilename(baseFilename, currentFile);
                if (in->is_open())
                {
                    in->close();
                }
                in->open(filename);

                in->read((char*) &numRemaining, sizeof(uint32));
                startedReading = true;
            }

            while (numRemaining > 0 && size < maxSize)
            {
                uint32 keySize;
                in->read((char*) &keySize, sizeof(uint32));
                bytelist key;
                key.resize(keySize);
                in->read(key.data(), keySize);

                uint32 valueSize;
                in->read((char*) &valueSize, sizeof(uint32));
                bytelist value;
                value.resize(valueSize);
                in->read(value.data(), valueSize);

                contents.push_back(KeyValuePair(key,value));
                size += keySize;
                size += valueSize;
                --numRemaining;
            }

            if (numRemaining == 0)
            {
                ++currentFile;
                startedReading = false;
            }

            if (size >= maxSize)
            {
                break;
            }
        }

        std::reverse(contents.begin(), contents.end());
    }

    SortedDiskCache::SortedDiskCache( std::string baseFilename_
                                    , uint64 maxSize_
                                    , Comparator comparator_
                                    ) : UnsortedDiskCache(baseFilename_, maxSize_)
    {
        comparator = comparator_;
    }

    SortedDiskCache::~SortedDiskCache()
    {
        for (uint32 fileNumber = 0; fileNumber < numFiles; ++fileNumber)
        {
            std::string filename = generateFilename(baseFilename, fileNumber);
            std::remove(filename.c_str());
        }
    }

    void SortedDiskCache::flush()
    {
        std::sort(contents.begin(), contents.end(), comparator);
        UnsortedDiskCache::flush();
    }

    SortedDiskCache::Iterator SortedDiskCache::getIterator()
    {
        if (size > 0)
        {
            flush();
        }

        return SortedDiskCacheIterator(baseFilename, numFiles, maxSize, comparator);
    }

    SortedDiskCacheIterator::SortedDiskCacheIterator( std::string baseFilename_
                                                    , uint32 numFiles_
                                                    , uint64 maxSize_
                                                    , Comparator comparator_
                                                    ) : UnsortedDiskCacheIterator(baseFilename_, numFiles_, maxSize_)
    {
        comparator = comparator_;
        in = new std::ifstream[numFiles];
        numRemaining = new uint32[numFiles];
        // TODO prep for the reads
        for (int currentFile = 0; currentFile < numFiles; ++currentFile)
        {
            std::string filename = generateFilename(baseFilename, currentFile);
            in[currentFile].open(filename);
            in[currentFile].read((char*) &(numRemaining[currentFile]), sizeof(uint32));
        }

        std::cout << "READ IN:\n";
        for (int i = 0; i < numFiles; ++i)
        {
            std::cout << i << " " << numRemaining[i] << std::endl;
        }
    }

    SortedDiskCacheIterator::~SortedDiskCacheIterator()
    {
        delete [] in;
    }

    void SortedDiskCacheIterator::populateCache()
    {
        
    }
}

