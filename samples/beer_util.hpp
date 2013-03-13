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
        for (uint32 i = 0; i < name.size(); ++i, ++ptr)
        {
            *ptr = name[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = style.size() + 1;
        ptr += sizeof(uint32);
        for (uint32 i = 0; i < style.size(); ++i, ++ptr)
        {
            *ptr = style[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = reviewerName.size() + 1;
        ptr += sizeof(uint32);
        for (uint32 i = 0; i < reviewerName.size(); ++i, ++ptr)
        {
            *ptr = reviewerName[i];
        }
        *ptr = '\0';
        ++ptr;

        *((uint32*) ptr) = reviewText.size() + 1;
        ptr += sizeof(uint32);
        for (uint32 i = 0; i < reviewText.size(); ++i, ++ptr)
        {
            *ptr = reviewText[i];
        }
        *ptr = '\0';
        ++ptr;
    }

    void deserialize(bytelist& bytes)
    {
        int8* ptr = bytes.data();

        beerId = *((uint32*) ptr);
        ptr += sizeof(uint32);

        brewerId = *((uint32*) ptr);
        ptr += sizeof(uint32);

        time = *((uint32*) ptr);
        ptr += sizeof(uint32);

        ABV = *((float*) ptr);
        ptr += sizeof(float);

        appearance = *((float*) ptr);
        ptr += sizeof(float);

        aroma = *((float*) ptr);
        ptr += sizeof(float);

        palate = *((float*) ptr);
        ptr += sizeof(float);

        taste = *((float*) ptr);
        ptr += sizeof(float);

        overall = *((float*) ptr);
        ptr += sizeof(float);

        uint32 nameSize = *((uint32*) ptr);
        ptr += sizeof(uint32);
        char* namePtr = ptr;
        ptr += nameSize;
        name = std::string(namePtr);

        uint32 styleSize = *((uint32*) ptr);
        ptr += sizeof(uint32);
        char* stylePtr = ptr;
        ptr += styleSize;
        style = std::string(stylePtr);

        uint32 reviewerNameSize = *((uint32*) ptr);
        ptr += sizeof(uint32);
        char* reviewerNamePtr = ptr;
        ptr += reviewerNameSize;
        reviewerName = std::string(reviewerNamePtr);

        uint32 reviewTextSize = *((uint32*) ptr);
        ptr += sizeof(uint32);
        char* reviewTextPtr = ptr;
        ptr += reviewTextSize;
        reviewText = std::string(reviewTextPtr);
    }

    void fromStringStream(std::stringstream& record)
    {
        std::string field;

        record >> field;
        getline(record, name);

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> beerId;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> brewerId;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> ABV;
        }

        record >> field;
        getline(record, style);

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> appearance;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> aroma;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> palate;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> taste;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> overall;
        }

        record >> field;
        {
            std::string line;
            getline(record, line);
            std::stringstream sstream(line);
            sstream >> time;
        }

        record >> field;
        getline(record, reviewerName);

        record >> field;
        getline(record, reviewText);
    }
};

bool lexicographicalCompare( const KeyValuePair& left
                           , const KeyValuePair& right
                           )
{
    return left.key < right.key;
}

