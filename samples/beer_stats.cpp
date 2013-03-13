#include "job.hpp"
#include "types.hpp"
#include "mapper.hpp"
#include "reducer.hpp"
#include "diskcache.hpp"
#include "diskcache.hpp"
#include "inputreader.hpp"
#include "impl/inputreaders.hpp"

#include <iostream>
#include <string>
#include <sstream>

using namespace mr;

struct BeerReview
{
    std::string name;
    uint32 beerId;
    uint32 brewerId;
    float ABV;
    std::string style;
    float appearance;
    float aroma;
    float palate;
    float taste;
    float overall;
    uint32 time;
    std::string reviewerName;
    std::string reviewText;

    uint32 getSize()
    {
        return 3 * sizeof(uint32)
             + 6 * sizeof(float)
             + 4 * sizeof(uint32)
             + name.size()
             + style.size()
             + reviewerName.size()
             + reviewText.size()
             + 4;
    }

    void serialize(bytelist& bytes)
    {
        bytes.resize(getSize());
        int8* ptr = bytes.data();

        *((uint32*) ptr) = beerId;
        ptr += sizeof(uint32);

        *((uint32*) ptr) = brewerId;
        ptr += sizeof(uint32);

        *((uint32*) ptr) = time;
        ptr += sizeof(uint32);

        *((float*) ptr) = ABV;
        ptr += sizeof(float);

        *((float*) ptr) = appearance;
        ptr += sizeof(float);

        *((float*) ptr) = aroma;
        ptr += sizeof(float);

        *((float*) ptr) = palate;
        ptr += sizeof(float);

        *((float*) ptr) = taste;
        ptr += sizeof(float);

        *((float*) ptr) = overall;
        ptr += sizeof(float);

        *((uint32*) ptr) = name.size() + 1;
        ptr += sizeof(uint32);
        for (int i = 0; i < name.size(); ++i, ++ptr)
        {
            *ptr = name[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = style.size() + 1;
        ptr += sizeof(uint32);
        for (int i = 0; i < style.size(); ++i, ++ptr)
        {
            *ptr = style[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = reviewerName.size() + 1;
        ptr += sizeof(uint32);
        for (int i = 0; i < reviewerName.size(); ++i, ++ptr)
        {
            *ptr = reviewerName[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = reviewText.size() + 1;
        ptr += sizeof(uint32);
        for (int i = 0; i < reviewText.size(); ++i, ++ptr)
        {
            *ptr = reviewText[i];
        }
        *ptr = '\0';
        ++ptr;
    }

    void deserialize(bytelist& bytes)
    {
        // TODO implement this
    }

    void fromStringStream(std::stringstream& record)
    {
        std::string field;

        record >> field;
        getline(record, name);

        record >> field;
        record >> beerId;

        record >> field;
        record >> brewerId;

        record >> field;
        record >> ABV;

        record >> field;
        getline(record, style);

        record >> field;
        record >> appearance;

        record >> field;
        record >> aroma;

        record >> field;
        record >> palate;

        record >> field;
        record >> taste;

        record >> field;
        record >> overall;

        record >> field;
        record >> time;

        record >> field;
        getline(record, reviewerName);

        record >> field;
        getline(record, reviewText);
    }
};

void mapFunction( KeyValuePair& pair
                , OutputCollector* collector
                )
{
    std::stringstream record(pair.value.data());
    BeerReview review;
    review.fromStringStream(record);

    KeyValuePair result;
    uint32 size = review.getSize();
    review.serialize(result.key);
    review.serialize(result.value);
    collector->collect(result);
}

void reduceFunction( bytelist& key
                   , std::vector<bytelist>& values
                   , OutputCollector* collector
                   )
{

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

    MapReduceJob job(numMappers, mapFunction, numReducers, reduceFunction, comparator, mapperInput, memoryLimit);
    job.run();
}

