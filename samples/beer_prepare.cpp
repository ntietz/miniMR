#include "beer_util.hpp"

void mapFunction( KeyValuePair& pair
                , OutputCollector* collector
                )
{
    std::stringstream record(pair.value.data());
    BeerReview review;
    review.fromStringStream(record);

    KeyValuePair result;
    review.serialize(result.key);
    result.value.resize(0);
    collector->collect(result);
}

void reduceFunction( bytelist& key
                   , std::vector<bytelist>& values
                   , OutputCollector* collector
                   )
{
    BeerReview review;
    review.deserialize(key);

    KeyValuePair result;
    result.key.resize(sizeof(uint32));
    *((uint32*) result.key.data()) = review.beerId;
    result.value = key;
    collector->collect(result);
}

int main(int argc, char** argv)
{
    uint64 memoryLimit = 4 * GIGABYTE;
    uint32 numMappers = 4;
    uint32 numReducers = 4;

    if (argc < 4)
    {
        std::cout << "Usage:" << std::endl
                  << "  beer_stats num_mappers num_reducers data_filename" << std::endl;
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

    std::cout << "Memory limit: " << memoryLimit << " bytes." << std::endl;
    std::cout << "Num mappers:  " << numMappers << std::endl;
    std::cout << "Num reducers: " << numReducers << std::endl;
    std::cout << std::endl;

    char* filename = argv[3];
    MapperInput* mapperInput = new ParagraphInputReader(filename);

    MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, lexicographicalCompare, mapperInput, memoryLimit);
    job.run();

    job.getResults()->getIterator(memoryLimit / 10);
}

