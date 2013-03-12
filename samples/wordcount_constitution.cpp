#include "job.hpp"
#include "types.hpp"
#include "mapper.hpp"
#include "reducer.hpp"
#include "diskcache.hpp"
#include "diskcache.hpp"
#include "inputreader.hpp"
#include "impl/inputreaders.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <functional>

using namespace mr;

void toUppercase(std::string&);
void toLetters(std::string&);

void mapFunction( KeyValuePair& pair
                , OutputCollector* collector
                )
{
    //int& lineNumber = *((int*) pair.key.data());

    std::stringstream sstream(pair.value.data());
    std::string word;

    while (sstream >> word)
    {
        toLetters(word);
        toUppercase(word);
        KeyValuePair outputPair;
        outputPair.key.resize(word.size() + 1);
        for (uint32 i = 0; i < word.size(); ++i)
        {
            outputPair.key[i] = word[i];
        }
        outputPair.key[word.size()] = '\0';

        outputPair.value.resize(sizeof(uint32));
        uint32& num = *((uint32*) outputPair.value.data());
        num = 1;

        collector->collect(outputPair);
    }
}

void reduceFunction( bytelist& key
                   , std::vector<bytelist>& values
                   , OutputCollector* collector
                   )
{
    uint32 sum = 0;
    for (auto& each : values)
    {
        uint32& val = *((uint32*) each.data());
        sum += val;
    }

    KeyValuePair outputPair;
    outputPair.key = key;
    outputPair.value.resize(sizeof(uint32));
    int& value = *((int*) outputPair.value.data());
    value = sum;

    collector->collect(outputPair);
}

bool comparator( const KeyValuePair& left
               , const KeyValuePair& right
               )
{
    return left.key < right.key;
}

int main(int argc, char** argv)
{
    uint64 memoryLimit = 4 * GIGABYTE;
    uint32 numMappers = 4;
    uint32 numReducers = 4;

    std::mutex outlock;

    if (argc < 2)
    {
        std::cout << "Error: you must provide a filename." << std::endl;
        return 1;
    }

    char* filename = argv[1];
    MapperInput* mapperInput = new LineInputReader(filename);
    //MapperInput* mapperInput = new LineInputReader("test/data/constitution.txt");

    MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, comparator, mapperInput, memoryLimit);
    job.run();

    std::ofstream out("results.txt");

    UnsortedDiskCache* resultCache = job.getResults();
    DiskCacheIterator resultIterator = resultCache->getIterator(memoryLimit);
    int max = 0;
    std::string mostFrequent;
    while (resultIterator.hasNext())
    {
        KeyValuePair pair = resultIterator.getNext();
        char* word = pair.key.data();
        int& count = *((int*) pair.value.data());
        if (count > max)
        {
            max = count;
            mostFrequent = word;
        }

        out << word << "," << count << std::endl;
    }

    std::cout << "Most frequent: " << mostFrequent << ", " << max << std::endl;

    delete mapperInput;
    delete job.getResults();
}

void toUppercase(std::string& word)
{
    for (uint32 i = 0; i < word.size(); ++i)
    {
        if (('a' <= word[i] && word[i] <= 'z'))
        {
            word[i] = (word[i] - 'a' + 'A');
        }
    }
}

void toLetters(std::string& word)
{
    for (uint32 i = 0; i < word.size(); ++i)
    {
        if (!(('a' <= word[i] && word[i] <= 'z') || ('A' <= word[i] && word[i] <= 'Z')))
        {
            word.erase(i,1);
            --i;
        }
    }
}

