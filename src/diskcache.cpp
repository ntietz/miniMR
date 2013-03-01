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

        return DiskCacheIterator(baseFilename, numFiles, maxSize);
    }

    DiskCacheIterator::DiskCacheIterator( std::string baseFilename_
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

    DiskCacheIterator::~DiskCacheIterator()
    {
        delete in;
    }

    bool DiskCacheIterator::hasNext()
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

    KeyValuePair DiskCacheIterator::getNext()
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

    void DiskCacheIterator::populateCache()
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

        std::remove(generateFilename(finalBaseFilename, 0).c_str());
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

        mergeFiles();

        // TODO add mergedBaseFilename
        return DiskCacheIterator(finalBaseFilename, 1, maxSize);
    }

    //void SortedDiskCacheIterator::populateCache()
    void SortedDiskCache::mergeFiles()
    {
        std::vector<std::ifstream*> in;
        std::vector<uint32> numRemaining;
        uint32 totalRemaining = 0;

        finalBaseFilename = baseFilename + "merged";
        std::string finalFilename = generateFilename(finalBaseFilename, 0);
        std::ofstream out(finalFilename);

        uint64 maxBufferSize = maxSize / numFiles;
        std::vector<KeyValuePair>* buffers = new std::vector<KeyValuePair>[numFiles];
        uint64* bufferSizes = new uint64[numFiles];

        for (int currentFile = 0; currentFile < numFiles; ++currentFile)
        {
            std::string filename = generateFilename(baseFilename, currentFile);
            in.push_back(new std::ifstream(filename));

            uint32 numCurrent;
            in[currentFile]->read((char*) &numCurrent, sizeof(uint32));

            numRemaining.push_back(numCurrent);
            totalRemaining += numCurrent;

            bufferSizes[currentFile] = 0;
        }

        out.write((char*) &totalRemaining, sizeof(uint32));

        auto fillBuffer = [&](uint32 currentFile)
        {
            while (bufferSizes[currentFile] < maxBufferSize && numRemaining[currentFile] > 0)
            {
                uint32 keySize;
                in[currentFile]->read((char*) &keySize, sizeof(uint32));
                bytelist key;
                key.resize(keySize);
                in[currentFile]->read(key.data(), keySize);

                uint32 valueSize;
                in[currentFile]->read((char*) &valueSize, sizeof(uint32));
                bytelist value;
                value.resize(valueSize);
                in[currentFile]->read(value.data(), valueSize);

                buffers[currentFile].push_back(KeyValuePair(key,value));

                bufferSizes[currentFile] += keySize;
                bufferSizes[currentFile] += valueSize;
                --numRemaining[currentFile];
                --totalRemaining;
            }
        };

        auto outputPair = [&](KeyValuePair& pair)
        {
            uint32 keySize = pair.key.size();
            out.write((char*) &keySize, sizeof(uint32));
            out.write(pair.key.data(), keySize);

            uint32 valueSize = pair.value.size();
            out.write((char*) &valueSize, sizeof(uint32));
            out.write(pair.value.data(), valueSize);
        };

        while (totalRemaining > 0)
        {
            for (uint32 currentFile = 0; currentFile < numFiles; ++currentFile)
            {
                fillBuffer(currentFile);
                int numSorted = 0;
                for (uint32 i = 0; i < buffers[currentFile].size(); ++i)
                {
                    if (comparator(buffers[currentFile][i], buffers[currentFile][i+1]))
                    {
                        ++numSorted;
                    }
                }
            }

            // TODO do a numFiles-merge, outputting as we go
            bool allEmpty = false;
            while (!allEmpty)
            {
                uint32 minIndex = -1;
                KeyValuePair min;
                for (int currentFile = 0; currentFile < numFiles; ++currentFile)
                {
                    // TODO if buffer is empty, fill it up!
                    if (bufferSizes[currentFile] == 0)
                    {
                        fillBuffer(currentFile);
                    }

                    // TODO if the buffer is empty, continue!
                    if (bufferSizes[currentFile] == 0)
                    {
                        continue;
                    }

                    // TODO if min isn't set, set it!
                    if (minIndex == -1)
                    {
                        minIndex = currentFile;
                        min = buffers[currentFile][0];
                    }
                    // TODO check if the current one is lower
                    else if (comparator(buffers[currentFile][0], buffers[minIndex][0]))
                    {
                        minIndex = currentFile;
                        min = buffers[currentFile][0];
                    }
                }

                if (minIndex == -1)
                {
                    allEmpty = true;
                }
                else
                {
                    outputPair(min);
                    bufferSizes[minIndex] -= min.key.size();
                    bufferSizes[minIndex] -= min.value.size();
                    buffers[minIndex].erase(buffers[minIndex].begin());
                }
            }
        }

        delete [] buffers;
        delete [] bufferSizes;
    }
}

