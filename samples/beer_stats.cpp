#include "beer_util.hpp"

#include <vector>
#include <algorithm>

void mapFunction( KeyValuePair& pair
                , OutputCollector* collector
                )
{
    //uint32& beerId = *((uint32*) pair.key.data());
    BeerReview review;
    review.deserialize(pair.value);

    KeyValuePair result;

    std::string outputString = review.name;
    result.key.resize(outputString.size() + 1);
    for (uint32 i = 0; i < outputString.size(); ++i)
        result.key[i] = outputString[i];
    result.key[outputString.size()] = '\0';

    result.value.resize(sizeof(float));
    *((float*) result.value.data()) = review.overall;

    collector->collect(result);
}

void reduceFunction( bytelist& key
                   , std::vector<bytelist>& values
                   , OutputCollector* collector
                   )
{
    KeyValuePair result;

    uint32 size = values.size();
    float total = 0.0f;
    for (uint32 i = 0; i < values.size(); ++i)
    {
        float& score = *((float*) values[i].data());
        if (score > 0.0001f)
            total += score;
        else
            --size;
    }

    float average = total / size;

    result.key = key;
    result.value.resize(sizeof(float));
    *((float*) result.value.data()) = average;

    if (values.size() >= 100)
        collector->collect(result);
}

void print(std::ostream& out, KeyValuePair& pair)
{
    out << pair.key.data() << ": " << *((float*)pair.value.data()) << std::endl;
}

int main(int argc, char** argv)
{
    uint64 memoryLimit = 4 * GIGABYTE;
    uint32 numMappers = 4;
    uint32 numReducers = 4;

    if (argc < 3)
    {
        std::cout << "Usage:" << std::endl
                  << "  beer_stats num_mappers num_reducers" << std::endl;
        return 1;
    }

    {
        char* numMappersStr = argv[1];
        std::stringstream sstream(numMappersStr);
        sstream >> numMappers;
    } {
        char* numReducersStr = argv[2];
        std::stringstream sstream(numReducersStr);
        sstream >> numReducers;
    }

    DiskCacheIterator beerData("beerdata", 1, memoryLimit / 10);
    MapperInput* mapperInput = new DiskCacheReader(&beerData);

    MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, lexicographicalCompare, mapperInput, memoryLimit);
    job.run();

    std::ofstream out("results.txt");

    UnsortedDiskCache* resultCache = job.getResults();
    DiskCacheIterator resultIterator = resultCache->getIterator(memoryLimit);

    std::vector<std::pair<float, std::string>> results;

    while (resultIterator.hasNext())
    {
        KeyValuePair pair = resultIterator.getNext();
        float& score = *((float*)pair.value.data());
        std::string name(pair.key.data());
        results.push_back(std::pair<float,std::string>(score,name));
        //print(out, pair);
    }

    std::sort(results.begin(), results.end());

    for (auto& each : results)
    {
        out << each.second << ": " << each.first << std::endl;
    }

    delete mapperInput;
    delete job.getResults();
}

