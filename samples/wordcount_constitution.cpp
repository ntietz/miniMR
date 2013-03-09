#include "job.hpp"
#include "types.hpp"
#include "mapper.hpp"
#include "reducer.hpp"
#include "diskcache.hpp"
#include "diskcache.hpp"
#include "inputreader.hpp"
#include "impl/inputreaders.hpp"

#include <sstream>
#include <iostream>
#include <cstring>
#include <functional>

using namespace mr;

void toUppercase(std::string&);
void toLetters(std::string&);

int main()
{
    uint32 memoryLimit = 4 * GIGABYTE;
    uint32 numMappers = 4;
    uint32 numReducers = 4;

    MapFunction mapFunction = []( KeyValuePair& pair
                                , OutputCollector* collector
                                )
    {
        int& lineNumber = *((int*) pair.key.data());

        std::stringstream sstream(pair.value.data());
        std::string word;

        while (sstream >> word)
        {
            toLetters(word);
            toUppercase(word);
            KeyValuePair outputPair;
            outputPair.key.resize(word.size() + 1);
            for (int i = 0; i < word.size(); ++i)
            {
                outputPair.key[i] = word[i];
            }
            outputPair.key[word.size()] = '\0';

            outputPair.value.resize(sizeof(uint32));
            uint32& num = *((uint32*) outputPair.value.data());
            num = 1;

            collector->collect(outputPair);
        }
    };

    std::mutex outlock;
    ReduceFunction reduceFunction = [&]( bytelist& key
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

        outlock.lock();
        std::cout << key.data() << ": " << sum << std::endl;
        outlock.unlock();
    };

    Comparator comparator = []( const KeyValuePair& left
                              , const KeyValuePair& right
                              )
    {
        return left.key < right.key;
        //return strcmp(left.key.data(), right.key.data()) < 0;
    };

    MapperInput* mapperInput = new LineInputReader("test/data/constitution.txt");

    MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, comparator, mapperInput, 102400000);
    job.run();

    delete mapperInput;
    delete job.getResults();
}

void toUppercase(std::string& word)
{
    for (int i = 0; i < word.size(); ++i)
    {
        if (('a' <= word[i] && word[i] <= 'z'))
        {
            word[i] = (word[i] - 'a' + 'A');
        }
    }
}

void toLetters(std::string& word)
{
    for (int i = 0; i < word.size(); ++i)
    {
        if (!(('a' <= word[i] && word[i] <= 'z') || ('A' <= word[i] && word[i] <= 'Z')))
        {
            word.erase(i,1);
            --i;
        }
    }
}

