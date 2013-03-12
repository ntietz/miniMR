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

    UnsortedDiskCache::Iterator UnsortedDiskCache::getIterator(uint64 memoryLimit_)
    {
        if (size > 0)
        {
            flush();
        }

        return DiskCacheIterator(baseFilename, numFiles, memoryLimit_);
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

    KeyValuePair& DiskCacheIterator::peek()
    {
        return contents.back();
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
                bytelist key(keySize);
                key.resize(keySize);
                in->read(key.data(), keySize);

                uint32 valueSize;
                in->read((char*) &valueSize, sizeof(uint32));
                bytelist value(valueSize);
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
        auto fasterComparator = [&](const uint32& leftIndex, const uint32& rightIndex)
        {
            return comparator(contents[leftIndex], contents[rightIndex]);
        };

        std::vector<uint32> indices;
        for (uint32 i = 0; i < contents.size(); ++i)
        {
            indices.push_back(i);
        }

        std::sort(indices.begin(), indices.end(), fasterComparator);

        // FLUSH
        int fileNumber = numFiles;
        std::string filename = generateFilename(baseFilename, fileNumber);

        std::ofstream out(filename.c_str());
        uint32 numberOfRecords = contents.size();
        out.write((char*) &numberOfRecords, sizeof(uint32));

        //for (uint32 index = 0; index < contents.size(); ++index)
        for (auto& index : indices)
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

    SortedDiskCache::Iterator SortedDiskCache::getIterator(uint64 memoryLimit_)
    {
        if (size > 0)
        {
            flush();
        }

        mergeFiles();

        return DiskCacheIterator(finalBaseFilename, 1, memoryLimit_);
    }

    void SortedDiskCache::mergeFiles()
    {
        std::ifstream* in = new std::ifstream[numFiles];
        uint32* numRemaining = new uint32[numFiles];
        uint32 totalRemaining = 0;

        finalBaseFilename = baseFilename + "merged";
        std::string finalFilename = generateFilename(finalBaseFilename, 0);
        std::ofstream out(finalFilename);

        if (numFiles == 0)
        {
            uint32 total = 0;
            out.write((char*) &total, sizeof(uint32));
            return;
        }

        uint64 maxBufferSize = maxSize / numFiles;
        std::vector<KeyValuePair>* buffers = new std::vector<KeyValuePair>[numFiles];
        uint32* bufferPositions = new uint32[numFiles];
        uint64* bufferSizes = new uint64[numFiles];

        for (uint32 currentFile = 0; currentFile < numFiles; ++currentFile)
        {
            std::string filename = generateFilename(baseFilename, currentFile);
            in[currentFile].open(filename);

            uint32 numCurrent;
            in[currentFile].read((char*) &numCurrent, sizeof(uint32));

            numRemaining[currentFile] = numCurrent;
            totalRemaining += numCurrent;

            bufferSizes[currentFile] = 0;
            bufferPositions[currentFile] = 0;
        }

        out.write((char*) &totalRemaining, sizeof(uint32));

        auto fillBuffer = [&](uint32 currentFile)
        {
            buffers[currentFile].clear();
            bufferPositions[currentFile] = 0;

            while (bufferSizes[currentFile] < maxBufferSize && numRemaining[currentFile] > 0)
            {
                uint32 keySize;
                in[currentFile].read((char*) &keySize, sizeof(uint32));
                bytelist key(keySize);
                key.resize(keySize);
                in[currentFile].read(key.data(), keySize);

                uint32 valueSize;
                in[currentFile].read((char*) &valueSize, sizeof(uint32));
                bytelist value(valueSize);
                value.resize(valueSize);
                in[currentFile].read(value.data(), valueSize);

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

            bool allEmpty = false;
            while (!allEmpty)
            {
                int32 minIndex = -1;
                KeyValuePair min;
                for (uint32 currentFile = 0; currentFile < numFiles; ++currentFile)
                {
                    if (bufferSizes[currentFile] == 0)
                    {
                        fillBuffer(currentFile);
                    }

                    if (bufferSizes[currentFile] == 0)
                    {
                        continue;
                    }

                    if (minIndex == -1)
                    {
                        minIndex = currentFile;
                        min = buffers[currentFile][bufferPositions[minIndex]];
                    }
                    else if (comparator( buffers[currentFile][bufferPositions[currentFile]]
                                       , buffers[minIndex][bufferPositions[minIndex]]))
                    {
                        minIndex = currentFile;
                        min = buffers[currentFile][bufferPositions[minIndex]];
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
                    bufferPositions[minIndex]++;
                    //buffers[minIndex].erase(buffers[minIndex].begin());
                }
            }
        }

        delete [] in;
        delete [] numRemaining;
        delete [] buffers;
        delete [] bufferPositions;
        delete [] bufferSizes;
    }
}

